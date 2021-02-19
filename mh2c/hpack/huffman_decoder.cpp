// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/huffman_decoder.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/huffman_code.h"

namespace mh2c {

namespace huffman {

encoded_value_t construct_integer(const byte_array_t& encoded_data,
                                  const size_t required_byte) {
  if (encoded_data.size() < required_byte) {
    const auto msg = "invalid argument: encoded_data.size()=" +
                     std::to_string(encoded_data.size()) +
                     ", required_byte=" + std::to_string(required_byte);
    throw std::invalid_argument(msg);
  }

  encoded_value_t value{};
  for (auto i = 0u; i < required_byte; ++i) {
    const auto left_shift_bit = (required_byte - i - 1) * 8;
    value |= (encoded_data.at(i) << left_shift_bit);
  }

  return value;
}

byte_array_t decode(const byte_array_t& encoded_data) {
  byte_array_t temp_encoded_data{encoded_data};
  byte_array_t decoded_data{};
  const size_t total_bit_length = temp_encoded_data.size() * 8;
  size_t decoded_bit_length{};
  std::pair<encoded_value_t, size_t> carry_over{};

  while (decoded_bit_length < total_bit_length) {
    auto found{false};

    for (const auto& entry : decode_table) {
      const auto bit_length = entry.first;
      if (temp_encoded_data.size() == 0 && carry_over.second < bit_length) {
        decoded_bit_length += carry_over.second;
        found = true;
        break;
      }

      encoded_value_t decode_map_key{};
      decltype(carry_over) temp_carry_over{};
      size_t erase_length{};
      if (carry_over.second < bit_length) {
        const auto required_bit = bit_length - carry_over.second;
        const auto required_byte =
            required_bit % 8 == 0 ? required_bit / 8 : required_bit / 8 + 1;
        auto target = construct_integer(temp_encoded_data, required_byte);
        const auto shift_bit =
            (required_byte * 8 + carry_over.second) - bit_length;
        const auto shifted_target = target >> shift_bit;
        const auto shifted_value = (target ^ (shifted_target << shift_bit));
        decode_map_key = (carry_over.first << (bit_length - carry_over.second)) |
                         shifted_target;
        temp_carry_over = {shifted_value, shift_bit};
        erase_length = required_byte;
      } else {
        auto target = carry_over.first;
        const auto shift_bit = carry_over.second - bit_length;
        const auto shifted_target = target >> shift_bit;
        const auto shifted_value = (target ^ (shifted_target << shift_bit));
        decode_map_key = shifted_target;
        temp_carry_over = {shifted_value, shift_bit};
        erase_length = 0;
      }

      const auto& decode_map = entry.second;
      const auto iter = decode_map.find(decode_map_key);
      if (iter != decode_map.end()) {
        found = true;
        decoded_data.push_back(iter->second);
        decoded_bit_length += bit_length;
        temp_encoded_data.erase(temp_encoded_data.begin(),
                                temp_encoded_data.begin() + erase_length);
        carry_over = temp_carry_over;
        break;
      }
    }

    if (found == false) {
      const auto msg =
          "decode failed: carry_over.first=" + std::to_string(carry_over.first) +
          ", carry_over.second=" + std::to_string(carry_over.second) +
          ", temp_encoded_data[0]=" + std::to_string(temp_encoded_data[0]) +
          ", temp_encoded_data[1]=" + std::to_string(temp_encoded_data[1]) +
          ", temp_encoded_data[2]=" + std::to_string(temp_encoded_data[2]) +
          ", temp_encoded_data[3]=" + std::to_string(temp_encoded_data[3]);
      throw std::runtime_error(msg);
    }
  }

  return decoded_data;
}

}  // namespace huffman

}  // namespace mh2c

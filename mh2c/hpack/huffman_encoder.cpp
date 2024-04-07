// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/huffman_encoder.h"

#include <algorithm>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/huffman_code.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

namespace huffman {

namespace {

constexpr auto FILL_BIT_SIZE = 8u;

void fill_record(encode_table_value* dst, encode_table_value* src) {
  const decltype(dst->second) lack = FILL_BIT_SIZE - dst->second;
  const auto fill_bit = std::min(lack, src->second);
  const auto remain_bit = src->second - fill_bit;
  const auto fill_data = src->first >> remain_bit;

  dst->first = (dst->first << fill_bit) | fill_data;
  dst->second += fill_bit;

  src->first ^= (fill_data << remain_bit);
  src->second = remain_bit;

  return;
}

template <typename T>
inline auto cast_to_byte_array_valut_type(T value) {
  return static_cast<byte_array_t::value_type>(value);
}

}  // namespace

byte_array_t encode(const byte_array_t& raw_data) {
  encode_table_value carry_over_record{};
  byte_array_t encoded_data{};

  const auto encode_func = [&carry_over_record,
                            &encoded_data](const auto data) {
    auto encode_table_record = encode_table.at(data);
    const encode_table_value::second_type total_bit_size =
        carry_over_record.second + encode_table_record.second;
    if (total_bit_size < FILL_BIT_SIZE) {
      fill_record(&carry_over_record, &encode_table_record);
      return;
    }

    for (auto remain_bit_size = total_bit_size;
         remain_bit_size >= FILL_BIT_SIZE;) {
      fill_record(&carry_over_record, &encode_table_record);

      encoded_data.push_back(
          cast_to_byte_array_valut_type(carry_over_record.first));
      carry_over_record = {0u, 0u};

      remain_bit_size = encode_table_record.second;
    }

    carry_over_record = encode_table_record;
    return;
  };

  std::for_each(raw_data.begin(), raw_data.end(), encode_func);
  if (carry_over_record.second > 0) {
    // cf. https://tools.ietf.org/html/rfc7540#section-6.5.1
    const auto eos_record = encode_table.at(encode_table.size() - 1);
    const auto pad_bit_size = FILL_BIT_SIZE - carry_over_record.second;
    const auto pad_bits =
        eos_record.first >> (eos_record.second - pad_bit_size);

    const auto last_byte = (carry_over_record.first << pad_bit_size) | pad_bits;
    encoded_data.push_back(cast_to_byte_array_valut_type(last_byte));
  }

  return encoded_data;
}

}  // namespace huffman

}  // namespace mh2c

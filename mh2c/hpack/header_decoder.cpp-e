// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/header_decoder.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/hpack/huffman_decoder.h"
#include "mh2c/hpack/integer_representation.h"
#include "mh2c/hpack/static_table_definition.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/cast.h"

namespace mh2c {

namespace {

using decoded_int_t = std::pair<size_t, size_t>;
using decoded_string_t = std::pair<std::string, size_t>;

constexpr uint8_t HEADER_PREFIX_BITS{4u};

}  // namespace

header_prefix_pattern check_prefix(const byte_array_t::value_type target) {
  std::vector<header_prefix_pattern> prefixes{
      header_prefix_pattern::INDEXED,
      header_prefix_pattern::INCREMENTAL_INDEXING,
      header_prefix_pattern::SIZE_UPDATE,
      header_prefix_pattern::NEVER_INDEXED,
      header_prefix_pattern::WITHOUT_INDEXING,
  };

  const auto target_prefix = extract_high_bit<HEADER_PREFIX_BITS>(target)
                             << (sizeof(target) * 8u - HEADER_PREFIX_BITS);
  for (const auto prefix : prefixes) {
    if (is_flag_set(target_prefix, prefix)) {
      return prefix;
    }
  }

  const auto msg = "target is invalid: " + std::to_string(target);
  throw std::invalid_argument(msg);
}

decoded_int_t decode_index(const byte_array_t& encoded_index,
                           const header_prefix_pattern prefix) {
  decoded_int_t::first_type index{};
  decoded_int_t::second_type decoded_byte_length{};

  switch (prefix) {
    case header_prefix_pattern::INDEXED: {
      constexpr auto bit_length{7u};
      index = decode_integer_value<bit_length>(encoded_index);
      // FIXME: how to get the number of decoded byte.
      decoded_byte_length = encode_integer_value<bit_length>(index).size();
      break;
    }
    case header_prefix_pattern::INCREMENTAL_INDEXING: {
      constexpr auto bit_length{6u};
      index = decode_integer_value<bit_length>(encoded_index);
      decoded_byte_length = encode_integer_value<bit_length>(index).size();
      break;
    }
    case header_prefix_pattern::WITHOUT_INDEXING: {
      constexpr auto bit_length{4u};
      index = decode_integer_value<bit_length>(encoded_index);
      decoded_byte_length = encode_integer_value<bit_length>(index).size();
      break;
    }
    case header_prefix_pattern::NEVER_INDEXED: {
      constexpr auto bit_length{4u};
      index = decode_integer_value<bit_length>(encoded_index);
      decoded_byte_length = encode_integer_value<bit_length>(index).size();
      break;
    }
    default:
      const auto msg =
          "prefix is invalid: " + std::to_string(underlying_cast(prefix));
      throw std::invalid_argument(msg);
      break;
  }

  return {index, decoded_byte_length};
}

decoded_int_t decode_max_size(const byte_array_t& encoded_max_size) {
  constexpr auto bit_length{5u};
  decoded_int_t::first_type max_size{
      decode_integer_value<bit_length>(encoded_max_size)};
  decoded_int_t::second_type decoded_byte_length =
      encode_integer_value<bit_length>(max_size).size();
  return {max_size, decoded_byte_length};
}

decoded_string_t decode_header_name(const byte_array_t& encoded_header_name) {
  // decode length of header name
  constexpr auto bit_length{7u};
  const auto name_length =
      decode_integer_value<bit_length>(encoded_header_name);
  auto decoded_byte_length =
      encode_integer_value<bit_length>(name_length).size();

  // decode header name
  const auto is_huffman_encode = extract_high_bit<1>(encoded_header_name[0]);
  std::string header_name{};
  if (is_huffman_encode) {
    const auto begin = encoded_header_name.begin() + decoded_byte_length;
    byte_array_t huffman_encoded_data{begin, begin + name_length};
    const auto huffman_decoded_data = huffman::decode(huffman_encoded_data);
    header_name = {reinterpret_cast<const char*>(&huffman_decoded_data[0]),
                   huffman_decoded_data.size()};
  } else {
    header_name = {reinterpret_cast<const char*>(
                       &encoded_header_name[decoded_byte_length]),
                   name_length};
  }
  decoded_byte_length += name_length;

  return {header_name, decoded_byte_length};
}

decoded_string_t decode_header_value(const byte_array_t& encoded_header_value) {
  // decode length of header value
  constexpr auto bit_length{7u};
  const auto value_length =
      decode_integer_value<bit_length>(encoded_header_value);
  auto decoded_byte_length =
      encode_integer_value<bit_length>(value_length).size();

  // decode header name
  const auto is_huffman_encode = extract_high_bit<1>(encoded_header_value[0]);
  std::string header_value{};
  if (is_huffman_encode) {
    const auto begin = encoded_header_value.begin() + decoded_byte_length;
    byte_array_t huffman_encoded_data{begin, begin + value_length};
    const auto huffman_decoded_data = huffman::decode(huffman_encoded_data);
    header_value = {reinterpret_cast<const char*>(&huffman_decoded_data[0]),
                    huffman_decoded_data.size()};
  } else {
    header_value = {reinterpret_cast<const char*>(
                        &encoded_header_value[decoded_byte_length]),
                    value_length};
  }
  decoded_byte_length += value_length;

  return {header_value, decoded_byte_length};
}

header_t make_indexed_header(const size_t index,
                             const dynamic_table& dynamic_table) {
  const auto static_table_end_index = static_table_entries.size();
  const auto header =
      (index > static_table_end_index)
          ? header_t{dynamic_table.at(index - static_table_end_index - 1)}
          : header_t{static_table_entries.at(index)};
  return header;
}

decoded_header_t decode_header(const byte_array_t& encoded_header,
                               const dynamic_table& dynamic_table) {
  byte_array_t encoded_data{encoded_header};
  const auto prefix = check_prefix(encoded_data[0]);

  // decode max size
  if (prefix == header_prefix_pattern::SIZE_UPDATE) {
    const auto [max_size, decoded_length] = decode_max_size(encoded_data);
    const header_block_entry entry{prefix, max_size};
    return {entry, decoded_length};
  }

  // decode index
  const auto [index, index_byte_length] = decode_index(encoded_data, prefix);
  const auto indexed_header =
      (index > 0 ? header_block_entry{prefix,
                                      make_indexed_header(index, dynamic_table)}
                 : header_block_entry{prefix, {"", ""}});
  if (prefix == header_prefix_pattern::INDEXED) {
    return {indexed_header, index_byte_length};
  }

  encoded_data.erase(encoded_data.begin(),
                     encoded_data.begin() + index_byte_length);
  decoded_header_t::second_type decoded_byte_length = index_byte_length;

  // decode header name
  decoded_header_t::first_type header_entry{indexed_header};
  auto header = header_entry.get_header();
  if (header.first.length() <= 0) {
    const auto [header_name, header_name_byte_length] =
        decode_header_name(encoded_data);
    encoded_data.erase(encoded_data.begin(),
                       encoded_data.begin() + header_name_byte_length);
    decoded_byte_length += header_name_byte_length;
    header.first = header_name;
  }

  // decode header name
  const auto [header_value, header_value_byte_length] =
      decode_header_value(encoded_data);
  encoded_data.erase(encoded_data.begin(),
                     encoded_data.begin() + header_value_byte_length);
  decoded_byte_length += header_value_byte_length;
  header.second = header_value;

  header_entry.set_header(header);

  return {header_entry, decoded_byte_length};
}

}  // namespace mh2c

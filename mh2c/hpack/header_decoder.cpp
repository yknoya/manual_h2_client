// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/header_decoder.h"

#include <array>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_decoder_internal.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/hpack/huffman_decoder.h"
#include "mh2c/hpack/static_table_definition.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/cast.h"

namespace mh2c {

namespace {

using decoded_int_t = std::pair<size_t, size_t>;
using decoded_string_t = std::pair<std::string, size_t>;

constexpr uint8_t HEADER_PREFIX_BITS{4u};

using byte_iter_t = byte_array_t::const_iterator;

template <uint8_t PrefixBits>
decoded_int_t decode_prefixed_integer(byte_iter_t begin, byte_iter_t end) {
  if (begin == end) {
    throw std::invalid_argument("encoded integer is empty");
  }

  constexpr auto kPrefixMask = static_cast<uint8_t>((1u << PrefixBits) - 1u);
  constexpr uint8_t kContinuationMask = 0x80u;
  constexpr uint8_t kValueMask = 0x7fu;

  auto decoded_value = static_cast<size_t>(*begin & kPrefixMask);
  size_t decoded_byte_length{1u};
  if (decoded_value < kPrefixMask) {
    return {decoded_value, decoded_byte_length};
  }

  size_t shift_bit{};
  for (auto iter = std::next(begin);; ++iter) {
    if (iter == end) {
      throw std::invalid_argument("truncated integer representation");
    }

    const auto next_byte = *iter;
    decoded_value += static_cast<size_t>(next_byte & kValueMask) << shift_bit;
    ++decoded_byte_length;
    if ((next_byte & kContinuationMask) != kContinuationMask) {
      break;
    }

    shift_bit += 7u;
  }

  return {decoded_value, decoded_byte_length};
}

decoded_string_t decode_string(byte_iter_t begin, byte_iter_t end,
                               const char* label) {
  if (begin == end) {
    throw std::invalid_argument(std::string(label) + " is empty");
  }

  const auto [value_length, decoded_byte_length] =
      decode_prefixed_integer<7u>(begin, end);
  if (static_cast<size_t>(std::distance(begin, end)) <
      decoded_byte_length + value_length) {
    throw std::invalid_argument("truncated " + std::string(label));
  }

  const auto is_huffman_encode = extract_high_bit<1>(*begin);
  const auto value_begin = std::next(begin, decoded_byte_length);
  if (is_huffman_encode) {
    byte_array_t huffman_encoded_data{value_begin,
                                      std::next(value_begin, value_length)};
    const auto decoded_data = huffman::decode(huffman_encoded_data);
    return {{reinterpret_cast<const char*>(decoded_data.data()),
             decoded_data.size()},
            decoded_byte_length + value_length};
  }

  if (value_length == 0) {
    return {"", decoded_byte_length};
  }

  return {{reinterpret_cast<const char*>(&*value_begin), value_length},
          decoded_byte_length + value_length};
}

}  // namespace

header_prefix_pattern check_prefix(const byte_array_t::value_type target) {
  constexpr std::array<header_prefix_pattern, 5u> prefixes{
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

decoded_int_t decode_index(byte_iter_t begin, byte_iter_t end,
                           const header_prefix_pattern prefix) {
  switch (prefix) {
    case header_prefix_pattern::INDEXED:
      return decode_prefixed_integer<7u>(begin, end);
    case header_prefix_pattern::INCREMENTAL_INDEXING:
      return decode_prefixed_integer<6u>(begin, end);
    case header_prefix_pattern::WITHOUT_INDEXING:
    case header_prefix_pattern::NEVER_INDEXED:
      return decode_prefixed_integer<4u>(begin, end);
    default:
      const auto msg =
          "prefix is invalid: " + std::to_string(underlying_cast(prefix));
      throw std::invalid_argument(msg);
  }
}

decoded_int_t decode_max_size(byte_iter_t begin, byte_iter_t end) {
  return decode_prefixed_integer<5u>(begin, end);
}

decoded_string_t decode_header_name(byte_iter_t begin, byte_iter_t end) {
  return decode_string(begin, end, "header name");
}

decoded_string_t decode_header_value(byte_iter_t begin, byte_iter_t end) {
  return decode_string(begin, end, "header value");
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

decoded_header_t decode_header_range(byte_iter_t begin, byte_iter_t end,
                                     const dynamic_table& dynamic_table) {
  if (begin == end) {
    throw std::invalid_argument("encoded header is empty");
  }
  const auto prefix = check_prefix(*begin);

  if (prefix == header_prefix_pattern::SIZE_UPDATE) {
    const auto [max_size, decoded_byte_length] = decode_max_size(begin, end);
    return {header_block_entry{prefix, max_size}, decoded_byte_length};
  }

  const auto [index, index_byte_length] = decode_index(begin, end, prefix);
  const auto indexed_header =
      (index > 0 ? header_block_entry{prefix,
                                      make_indexed_header(index, dynamic_table)}
                 : header_block_entry{prefix, {"", ""}});
  if (prefix == header_prefix_pattern::INDEXED) {
    return {indexed_header, index_byte_length};
  }

  decoded_header_t::second_type decoded_byte_length = index_byte_length;
  auto current = std::next(begin, index_byte_length);

  decoded_header_t::first_type header_entry{indexed_header};
  auto header = header_entry.get_header();
  if (header.first.empty()) {
    const auto [header_name, header_name_byte_length] =
        decode_header_name(current, end);
    decoded_byte_length += header_name_byte_length;
    current += header_name_byte_length;
    header.first = header_name;
  }

  const auto [header_value, header_value_byte_length] =
      decode_header_value(current, end);
  decoded_byte_length += header_value_byte_length;
  header.second = header_value;

  header_entry.set_header(header);

  return {header_entry, decoded_byte_length};
}

decoded_header_t decode_header(const byte_array_t& encoded_header,
                               const dynamic_table& dynamic_table) {
  return decode_header_range(encoded_header.begin(), encoded_header.end(),
                             dynamic_table);
}

}  // namespace mh2c

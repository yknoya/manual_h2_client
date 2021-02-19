// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/header_encoder.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/hpack/huffman_encoder.h"
#include "mh2c/hpack/integer_representation.h"
#include "mh2c/hpack/static_table_definition.h"

namespace mh2c {

byte_array_t encode_index(const size_t index,
                          const header_prefix_pattern prefix) {
  byte_array_t encoded_index{};

  switch (prefix) {
    case header_prefix_pattern::INDEXED:
      encoded_index = encode_integer_value<7>(index);
      encoded_index[0] |= static_cast<byte_array_t::value_type>(prefix);
      break;
    case header_prefix_pattern::INCREMENTAL_INDEXING:
      encoded_index = encode_integer_value<6>(index);
      encoded_index[0] |= static_cast<byte_array_t::value_type>(prefix);
      break;
    case header_prefix_pattern::WITHOUT_INDEXING:
    case header_prefix_pattern::NEVER_INDEXED:
      encoded_index = encode_integer_value<4>(index);
      encoded_index[0] |= static_cast<byte_array_t::value_type>(prefix);
      break;
    default:
      const auto msg =
          "prefix is invalid: " +
          std::to_string(static_cast<byte_array_t::value_type>(prefix));
      throw std::invalid_argument(msg);
      break;
  }

  return encoded_index;
}

byte_array_t encode_max_size(const size_t max_size,
                             const header_prefix_pattern prefix) {
  byte_array_t encoded_max_size = encode_integer_value<5>(max_size);
  encoded_max_size[0] |= static_cast<byte_array_t::value_type>(prefix);
  return encoded_max_size;
}

byte_array_t encode_header_value(const header_value_t& header_value,
                                 const header_encode_mode encode_mode) {
  byte_array_t encoded_header_value{};

  // Apply Huffman encode if needed
  const byte_array_t header_value_bytes{header_value.begin(), header_value.end()};
  const auto encoded_bytes = (encode_mode == header_encode_mode::HUFFMAN)
                                 ? huffman::encode(header_value_bytes)
                                 : header_value_bytes;

  // Emcpde header value length
  const auto flag_huffman_encode =
      static_cast<byte_array_t::value_type>(encode_mode);
  auto encoded_header_value_length =
      encode_integer_value<7>(encoded_bytes.size());
  encoded_header_value_length[0] |= flag_huffman_encode;
  std::copy(encoded_header_value_length.begin(),
            encoded_header_value_length.end(),
            std::back_inserter(encoded_header_value));

  // Encode header value
  std::copy(encoded_bytes.begin(), encoded_bytes.end(),
            std::back_inserter(encoded_header_value));

  return encoded_header_value;
}

byte_array_t encode_header_name(const header_name_t& header_name,
                                const header_encode_mode encode_mode) {
  byte_array_t encoded_header_name{};

  // Apply Huffman encode if needed
  const byte_array_t header_name_bytes{header_name.begin(), header_name.end()};
  const auto encoded_bytes = (encode_mode == header_encode_mode::HUFFMAN)
                                 ? huffman::encode(header_name_bytes)
                                 : header_name_bytes;

  // Emcpde header name length
  const auto flag_huffman_encode =
      static_cast<byte_array_t::value_type>(encode_mode);
  auto encoded_header_name_length = encode_integer_value<7>(encoded_bytes.size());
  encoded_header_name_length[0] |= flag_huffman_encode;
  std::copy(encoded_header_name_length.begin(), encoded_header_name_length.end(),
            std::back_inserter(encoded_header_name));

  // Encode header value
  std::copy(encoded_bytes.begin(), encoded_bytes.end(),
            std::back_inserter(encoded_header_name));

  return encoded_header_name;
}

byte_array_t encode_header(const header_block_entry& header_entry,
                           const header_encode_mode encode_mode,
                           const dynamic_table& dynamic_table) {
  const auto prefix = header_entry.get_prefix();

  // max size update
  if (prefix == header_prefix_pattern::SIZE_UPDATE) {
    return encode_max_size(header_entry.get_max_size(), prefix);
  }

  const auto header = header_entry.get_header();

  // header name/value match in reverse static table
  // cf. https://tools.ietf.org/html/rfc7541#section-6.1
  const auto iter = reverse_static_table_entries.find(header);
  if (iter != reverse_static_table_entries.end() && header.second.length() > 0) {
    return encode_index(iter->second, header_prefix_pattern::INDEXED);
  }

  // header name/value match in dynamic table
  const auto header_iter =
      std::find(dynamic_table.begin(), dynamic_table.end(), header);
  if (header_iter != dynamic_table.end()) {
    const auto index = header_iter - dynamic_table.begin();
    return encode_index(static_table_entries.size() + 1 + index,
                        header_prefix_pattern::INDEXED);
  }

  // Only header name match
  // cf. https://tools.ietf.org/html/rfc7541#section-6.2.1
  //     https://tools.ietf.org/html/rfc7541#section-6.2.2
  //     https://tools.ietf.org/html/rfc7541#section-6.2.3
  header_t name_only_header{header.first, ""};
  const auto iter_name_only = reverse_static_table_entries.find(name_only_header);
  if (iter_name_only != reverse_static_table_entries.end()) {
    auto encoded_header = encode_index(iter_name_only->second, prefix);
    auto encoded_header_value = encode_header_value(header.second, encode_mode);
    std::copy(encoded_header_value.begin(), encoded_header_value.end(),
              std::back_inserter(encoded_header));
    return encoded_header;
  }

  // New header name
  // cf. https://tools.ietf.org/html/rfc7541#section-6.2.1
  //     https://tools.ietf.org/html/rfc7541#section-6.2.2
  //     https://tools.ietf.org/html/rfc7541#section-6.2.3
  auto encoded_header = encode_index(0u, prefix);
  auto encoded_header_name = encode_header_name(header.first, encode_mode);
  std::copy(encoded_header_name.begin(), encoded_header_name.end(),
            std::back_inserter(encoded_header));
  auto encoded_header_value = encode_header_value(header.second, encode_mode);
  std::copy(encoded_header_value.begin(), encoded_header_value.end(),
            std::back_inserter(encoded_header));
  return encoded_header;
}

}  // namespace mh2c

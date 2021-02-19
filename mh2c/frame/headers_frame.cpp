// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/headers_frame.h"

#include <algorithm>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_decoder.h"
#include "mh2c/hpack/header_encoder.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/hpack/integer_representation.h"
#include "mh2c/hpack/static_table_definition.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

/*
 * Anonymous namespace
 */
namespace {

constexpr uint8_t EXCLUSIVE_BITS{1u};

byte_array_t construct_encoded_payload(
    const fh_flags_t flags, const header_block_t& header_block,
    const header_encode_mode mode, const dynamic_table& dynamic_table,
    const byte_array_t& padding, const hf_priority_option& priority_option) {
  byte_array_t encoded_payload{};

  // Pad Length
  const auto is_padded_set = is_flag_set(flags, mh2c::hf_flag::PADDED);
  if (is_padded_set) {
    encoded_payload.push_back(padding.size());
  }

  // Priority Settings
  const auto is_priority_set = is_flag_set(flags, mh2c::hf_flag::PRIORITY);
  if (is_priority_set) {
    const uint32_t exclusive_and_stream_dependency =
        (priority_option.m_exclusive << STREAM_ID_BITS) |
        extract_low_bit<STREAM_ID_BITS>(priority_option.m_stream_dependency);
    const auto bytes =
        integral2bytes<byte_array_t>(exclusive_and_stream_dependency);
    std::copy(bytes.begin(), bytes.end(), std::back_inserter(encoded_payload));
    encoded_payload.push_back(priority_option.m_weight);
  }

  // Header Block
  std::for_each(
      header_block.begin(), header_block.end(),
      [&encoded_payload, mode, &dynamic_table](const auto& header_entry) {
        const auto encoded_header =
            encode_header(header_entry, mode, dynamic_table);
        std::copy(encoded_header.begin(), encoded_header.end(),
                  std::back_inserter(encoded_payload));
      });

  // Padding
  if (is_padded_set) {
    std::copy(padding.begin(), padding.end(),
              std::back_inserter(encoded_payload));
  }

  return encoded_payload;
}

frame_header construct_frame_header(const fh_flags_t flags,
                                    const fh_stream_id_t stream_id,
                                    const byte_array_t& encoded_payload) {
  return {static_cast<fh_length_t>(encoded_payload.size()),
          static_cast<fh_type_t>(frame_type_registry::HEADERS), flags, 0,
          stream_id};
}

struct decoded_payload_t {
  byte_array_t m_padding;
  hf_priority_option m_priority_option;
  header_block_t m_header_block;
};

decoded_payload_t decode_payload(const frame_header& fh,
                                 const byte_array_t& raw_payload,
                                 const dynamic_table& dynamic_table) {
  auto raw_data{raw_payload};
  header_block_t header_block{};

  // Extract padding if needed
  byte_array_t padding{};
  if ((fh.m_flags & static_cast<fh_flags_t>(hf_flag::PADDED)) ==
      static_cast<fh_flags_t>(hf_flag::PADDED)) {
    const auto pad_length = raw_data.front();
    const auto begin_padding = raw_data.end() - pad_length;
    const auto end_padding = raw_data.end();
    std::copy(begin_padding, end_padding, std::back_inserter(padding));

    raw_data.erase(begin_padding, end_padding);
    raw_data.erase(raw_data.begin());
  }

  // Extract parameters for priority
  hf_priority_option priority_option{};
  if ((fh.m_flags & static_cast<fh_flags_t>(hf_flag::PRIORITY)) ==
      static_cast<fh_flags_t>(hf_flag::PRIORITY)) {
    // Exclusive and Stream Dependency
    byte_array_t raw_stream_dependency{};
    const auto begin_stream_dependency = raw_data.begin();
    const auto end_stream_dependency =
        raw_data.begin() + sizeof(fh_stream_id_t);
    std::copy(begin_stream_dependency, end_stream_dependency,
              std::back_inserter(raw_stream_dependency));

    priority_option.m_exclusive =
        extract_high_bit<EXCLUSIVE_BITS>(raw_stream_dependency[0]);
    priority_option.m_stream_dependency = extract_low_bit<STREAM_ID_BITS>(
        bytes2integral<decltype(priority_option.m_stream_dependency)>(
            raw_stream_dependency.begin()));
    raw_data.erase(begin_stream_dependency, end_stream_dependency);

    // Weight
    priority_option.m_weight = raw_data.front();
    raw_data.erase(raw_data.begin());
  }

  // Header Block
  while (raw_data.size() > 0) {
    const auto decoded_header = decode_header(raw_data, dynamic_table);
    header_block.push_back(decoded_header.first);
    raw_data.erase(raw_data.begin(), raw_data.begin() + decoded_header.second);
  }

  return {padding, priority_option, header_block};
}

}  // namespace

/*
 * definitions of settings_frame
 */
headers_frame::headers_frame(const fh_flags_t flags,
                             const fh_stream_id_t stream_id,
                             const header_block_t& header_block,
                             const header_encode_mode mode,
                             const dynamic_table& dynamic_table,
                             const byte_array_t& padding,
                             const hf_priority_option& priority_option)
    : m_encoded_payload{construct_encoded_payload(
          flags, header_block, mode, dynamic_table, padding, priority_option)},
      m_header{construct_frame_header(flags, stream_id, m_encoded_payload)},
      m_padding{padding},
      m_priority_option{priority_option},
      m_header_block{header_block} {}

headers_frame::headers_frame(const frame_header& fh,
                             const byte_array_t& raw_payload,
                             const dynamic_table& dynamic_table)
    : m_encoded_payload{raw_payload}, m_header{fh} {
  const auto decoded_payload = decode_payload(fh, raw_payload, dynamic_table);
  m_padding = decoded_payload.m_padding;
  m_priority_option = decoded_payload.m_priority_option;
  m_header_block = decoded_payload.m_header_block;
}

frame_header headers_frame::get_header() const { return m_header; }

header_block_t headers_frame::get_payload() const { return m_header_block; }

byte_array_t headers_frame::serialize() const {
  byte_array_t serialized_hf = mh2c::serialize(m_header);
  std::copy(m_encoded_payload.begin(), m_encoded_payload.end(),
            std::back_inserter(serialized_hf));
  return serialized_hf;
}

void headers_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== HEADERS FRAME ===\n" << m_header << "[PAYLOAD]\n";

  const bool contain_priority =
      (m_header.m_flags & static_cast<fh_flags_t>(hf_flag::PRIORITY)) ==
      static_cast<fh_flags_t>(hf_flag::PRIORITY);
  const auto exclusive =
      contain_priority ? std::to_string(m_priority_option.m_exclusive) : "";
  const auto stream_dependency =
      contain_priority ? std::to_string(m_priority_option.m_stream_dependency)
                       : "";
  const auto weight =
      contain_priority ? std::to_string(m_priority_option.m_weight) : "";
  out_stream << "  Exclusive: " << exclusive << '\n';
  out_stream << "  Stream Dependency: " << stream_dependency << '\n';
  out_stream << "  Weight: " << weight << '\n';

  out_stream << "  Header Block:\n";
  std::for_each(
      m_header_block.begin(), m_header_block.end(),
      [&out_stream](const auto& header_entry) {
        if (header_entry.get_prefix() == header_prefix_pattern::SIZE_UPDATE) {
          out_stream << "    " << std::to_string(header_entry.get_max_size())
                     << " (dynamic table size update)\n";
          return;
        }
        const auto header = header_entry.get_header();
        out_stream << "    " << header.first << ": " << header.second << '\n';
      });

  out_stream << "  Padding:\n";
  const bool contain_padding = is_flag_set(m_header.m_flags, hf_flag::PADDED);
  if (contain_padding) {
    uint32_t counter{0};
    out_stream << "    ";
    std::for_each(m_padding.begin(), m_padding.end(),
                  [&out_stream, &counter](const auto& elem) {
                    out_stream << std::hex << std::setw(2) << std::setfill('0')
                               << static_cast<int>(elem);
                    if (++counter % 8 == 0) {
                      out_stream << '\n';
                      out_stream << "    ";
                    } else {
                      out_stream << ' ';
                    }
                  });
    out_stream << '\n';
  }

  return;
}

std::ostream& operator<<(std::ostream& out_stream, const headers_frame& hf) {
  hf.dump(out_stream);
  return out_stream;
}

bool operator==(const hf_priority_option& lhs, const hf_priority_option& rhs) {
  return lhs.m_exclusive == rhs.m_exclusive &&
         lhs.m_stream_dependency == rhs.m_stream_dependency &&
         lhs.m_weight == rhs.m_weight;
}

bool operator!=(const hf_priority_option& lhs, const hf_priority_option& rhs) {
  return !(lhs == rhs);
}

bool operator==(const headers_frame& lhs, const headers_frame& rhs) {
  return lhs.m_header == rhs.m_header &&
         lhs.m_priority_option == rhs.m_priority_option &&
         lhs.m_header_block == rhs.m_header_block &&
         lhs.m_padding == rhs.m_padding;
}

bool operator!=(const headers_frame& lhs, const headers_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

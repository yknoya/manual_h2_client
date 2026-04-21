// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/headers_frame.h"

#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/frame/header_block_utils.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"
#include "mh2c/util/cast.h"

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

  detail::append_encoded_header_block(&encoded_payload, header_block, mode,
                                      dynamic_table);

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
  return {cast_to_fh_length(encoded_payload.size()),
          underlying_cast(frame_type_registry::HEADERS), flags, 0, stream_id};
}

struct decoded_payload_t {
  byte_array_t m_padding;
  hf_priority_option m_priority_option;
  header_block_t m_header_block;
};

decoded_payload_t decode_payload(const frame_header& fh,
                                 const byte_array_t& raw_payload,
                                 const dynamic_table& dynamic_table) {
  auto payload_begin = raw_payload.begin();
  auto payload_end = raw_payload.end();

  byte_array_t padding{};
  if (is_flag_set(fh.m_flags, hf_flag::PADDED)) {
    if (payload_begin == payload_end) {
      throw std::invalid_argument("missing pad length");
    }

    const auto pad_length = *payload_begin;
    if (static_cast<size_t>(std::distance(payload_begin, payload_end)) <
        1u + pad_length) {
      throw std::invalid_argument("invalid padded payload");
    }
    ++payload_begin;
    payload_end -= pad_length;
    std::copy(payload_end, raw_payload.end(), std::back_inserter(padding));
  }

  hf_priority_option priority_option{};
  if (is_flag_set(fh.m_flags, hf_flag::PRIORITY)) {
    constexpr auto kPriorityBytes = sizeof(fh_stream_id_t) + sizeof(uint8_t);
    if (static_cast<size_t>(std::distance(payload_begin, payload_end)) <
        kPriorityBytes) {
      throw std::invalid_argument("invalid priority payload");
    }

    priority_option.m_exclusive =
        extract_high_bit<EXCLUSIVE_BITS>(*payload_begin);
    priority_option.m_stream_dependency = extract_low_bit<STREAM_ID_BITS>(
        bytes2integral<decltype(priority_option.m_stream_dependency)>(
            payload_begin));
    payload_begin += sizeof(fh_stream_id_t);

    priority_option.m_weight = *payload_begin;
    ++payload_begin;
  }

  const auto header_block =
      detail::decode_header_block(payload_begin, payload_end, dynamic_table);
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
      is_flag_set(m_header.m_flags, hf_flag::PRIORITY);
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
  detail::dump_header_block(out_stream, m_header_block);

  out_stream << "  Padding:\n";
  const bool contain_padding = is_flag_set(m_header.m_flags, hf_flag::PADDED);
  if (contain_padding) {
    detail::dump_hex_bytes(out_stream, m_padding);
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

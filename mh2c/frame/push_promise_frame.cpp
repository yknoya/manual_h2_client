// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/push_promise_frame.h"

#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/frame/header_block_utils.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"
#include "mh2c/util/cast.h"

namespace mh2c {

namespace {

byte_array_t construct_encoded_payload(const fh_flags_t flags,
                                       const push_promise_payload& payload,
                                       const header_encode_mode mode,
                                       const dynamic_table& dynamic_table) {
  byte_array_t encoded_payload{};

  // Pad Length
  const auto is_padded_set = is_flag_set(flags, mh2c::ppf_flag::PADDED);
  if (is_padded_set) {
    encoded_payload.push_back(payload.m_padding.size());
  }

  // Reserved and Promised Stream ID
  const decltype(payload.m_promised_stream_id) r_promised_stream_id =
      cvt_host2net(
          (extract_low_bit<RESERVED_BITS>(payload.m_reserved)
           << STREAM_ID_BITS) |
          extract_low_bit<STREAM_ID_BITS>(payload.m_promised_stream_id));
  const auto begin = reinterpret_cast<const uint8_t*>(&r_promised_stream_id);
  std::copy(begin, begin + sizeof(r_promised_stream_id),
            std::back_inserter(encoded_payload));

  detail::append_encoded_header_block(&encoded_payload, payload.m_header_block,
                                      mode, dynamic_table);

  // Padding
  if (is_padded_set) {
    std::copy(payload.m_padding.begin(), payload.m_padding.end(),
              std::back_inserter(encoded_payload));
  }

  return encoded_payload;
}

frame_header construct_frame_header(const fh_flags_t flags,
                                    const fh_stream_id_t stream_id,
                                    const size_t payload_size) {
  return {cast_to_fh_length(payload_size),
          underlying_cast(frame_type_registry::PUSH_PROMISE), flags, 0,
          stream_id};
}

push_promise_payload decode_payload(const frame_header& fh,
                                    const byte_array_t& raw_payload,
                                    const dynamic_table& dynamic_table) {
  auto payload_begin = raw_payload.begin();
  auto payload_end = raw_payload.end();

  byte_array_t padding{};
  if (is_flag_set(fh.m_flags, ppf_flag::PADDED)) {
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

  if (static_cast<size_t>(std::distance(payload_begin, payload_end)) <
      sizeof(fh_stream_id_t)) {
    throw std::invalid_argument("invalid promised stream id payload");
  }
  const reserved_t reserved = extract_high_bit<RESERVED_BITS>(*payload_begin);
  const auto promised_stream_id = extract_low_bit<STREAM_ID_BITS>(
      bytes2integral<fh_stream_id_t>(payload_begin));
  payload_begin += sizeof(fh_stream_id_t);

  const auto header_block =
      detail::decode_header_block(payload_begin, payload_end, dynamic_table);

  return {reserved, promised_stream_id, header_block, padding};
}  // namespace

}  // namespace

bool operator==(const push_promise_payload& lhs,
                const push_promise_payload& rhs) {
  return lhs.m_reserved == rhs.m_reserved &&
         lhs.m_promised_stream_id == rhs.m_promised_stream_id &&
         lhs.m_header_block == rhs.m_header_block &&
         lhs.m_padding == rhs.m_padding;
}

bool operator!=(const push_promise_payload& lhs,
                const push_promise_payload& rhs) {
  return !(lhs == rhs);
}

push_promise_frame::push_promise_frame(const fh_flags_t flags,
                                       const fh_stream_id_t stream_id,
                                       const push_promise_payload& payload,
                                       const header_encode_mode mode,
                                       const dynamic_table& dynamic_table)
    : m_encoded_payload{
          construct_encoded_payload(flags, payload, mode, dynamic_table)},
      m_header{
          construct_frame_header(flags, stream_id, m_encoded_payload.size())},
      m_payload{payload} {}

push_promise_frame::push_promise_frame(const frame_header& fh,
                                       const byte_array_t& raw_payload,
                                       const dynamic_table& dynamic_table)
    : m_encoded_payload{raw_payload},
      m_header{fh},
      m_payload{decode_payload(fh, raw_payload, dynamic_table)} {}

frame_header push_promise_frame::get_header() const { return m_header; }

push_promise_payload push_promise_frame::get_payload() const {
  return m_payload;
}

byte_array_t push_promise_frame::serialize() const {
  byte_array_t serialized_hf = mh2c::serialize(m_header);
  std::copy(m_encoded_payload.begin(), m_encoded_payload.end(),
            std::back_inserter(serialized_hf));
  return serialized_hf;
}

void push_promise_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== PUSH_PROMISE FRAME ===\n" << m_header << "[PAYLOAD]\n";
  out_stream << "  R: " << std::to_string(m_payload.m_reserved) << '\n';
  out_stream << "  Promised Stream ID: "
             << std::to_string(m_payload.m_promised_stream_id) << '\n';

  out_stream << "  Header Block:\n";
  detail::dump_header_block(out_stream, m_payload.m_header_block);

  out_stream << "  Padding:\n";
  const bool contain_padding = is_flag_set(m_header.m_flags, ppf_flag::PADDED);
  if (contain_padding) {
    detail::dump_hex_bytes(out_stream, m_payload.m_padding);
  }

  return;
}

std::ostream& operator<<(std::ostream& out_stream,
                         const push_promise_frame& hf) {
  hf.dump(out_stream);
  return out_stream;
}

bool operator==(const push_promise_frame& lhs, const push_promise_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_payload == rhs.m_payload;
}

bool operator!=(const push_promise_frame& lhs, const push_promise_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

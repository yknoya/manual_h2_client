// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/push_promise_frame.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_decoder.h"
#include "mh2c/hpack/header_encoder.h"
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

  // Header Block
  std::for_each(payload.m_header_block.begin(), payload.m_header_block.end(),
                [&encoded_payload, mode, &dynamic_table](const auto& header) {
                  const auto encoded_header =
                      encode_header(header, mode, dynamic_table);
                  std::copy(encoded_header.begin(), encoded_header.end(),
                            std::back_inserter(encoded_payload));
                });

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
  auto raw_data{raw_payload};

  // Extract Padding if needed
  byte_array_t padding{};
  if (is_flag_set(fh.m_flags, ppf_flag::PADDED)) {
    const auto pad_length = raw_data.front();
    const auto begin_padding = raw_data.end() - pad_length;
    const auto end_padding = raw_data.end();
    std::copy(begin_padding, end_padding, std::back_inserter(padding));

    raw_data.erase(begin_padding, end_padding);
    raw_data.erase(raw_data.begin());
  }

  // Extract Reserved and Promised Stream ID
  const reserved_t reserved = extract_high_bit<RESERVED_BITS>(raw_data[0]);
  const auto promised_stream_id = extract_low_bit<STREAM_ID_BITS>(
      bytes2integral<fh_stream_id_t>(raw_data.begin()));
  raw_data.erase(raw_data.begin(), raw_data.begin() + sizeof(fh_stream_id_t));

  // Header Block
  header_block_t header_block{};
  while (raw_data.size() > 0) {
    const auto decoded_header = decode_header(raw_data, dynamic_table);
    header_block.push_back(decoded_header.first);
    raw_data.erase(raw_data.begin(), raw_data.begin() + decoded_header.second);
  }

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
    : m_encoded_payload{construct_encoded_payload(flags, payload, mode,
                                                  dynamic_table)},
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
  std::for_each(
      m_payload.m_header_block.begin(), m_payload.m_header_block.end(),
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
  const bool contain_padding = is_flag_set(m_header.m_flags, ppf_flag::PADDED);
  if (contain_padding) {
    uint32_t counter{0};
    out_stream << "    ";
    std::for_each(m_payload.m_padding.begin(), m_payload.m_padding.end(),
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

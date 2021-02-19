// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/goaway_frame.h"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

namespace {

frame_header construct_frame_header(const goaway_payload& payload) {
  const fh_length_t length = sizeof(payload.m_last_stream_id) +
                             sizeof(payload.m_error_code) +
                             payload.m_additional_debug_data.size();

  return {length, static_cast<fh_type_t>(frame_type_registry::GOAWAY), 0u, 0u,
          0u};
}

goaway_payload construct_payload(const byte_array_t& raw_payload) {
  constexpr auto minimum_size = sizeof(fh_stream_id_t) + sizeof(error_codes);
  const auto payload_size = raw_payload.size();
  if (payload_size < minimum_size) {
    const auto msg =
        "invalid payload size: " + std::to_string(raw_payload.size());
    throw std::invalid_argument(msg);
  }

  // Reserved and Last-Stream-ID
  const auto r_and_last_stream_id =
      bytes2integral<fh_stream_id_t>(raw_payload.begin());
  uint8_t reserved = extract_high_bit<RESERVED_BITS>(r_and_last_stream_id);
  fh_stream_id_t last_stream_id =
      extract_low_bit<STREAM_ID_BITS>(r_and_last_stream_id);

  // Error Code
  const auto raw_error_code =
      bytes2integral<std::underlying_type_t<error_codes>>(
          raw_payload.begin() + sizeof(r_and_last_stream_id));
  const auto error_code = static_cast<error_codes>(raw_error_code);

  // Additional Debug Data
  byte_array_t additional_debug_data{};
  if (payload_size > minimum_size) {
    std::copy(raw_payload.begin() + minimum_size, raw_payload.end(),
              std::back_inserter(additional_debug_data));
  }

  return {reserved, last_stream_id, error_code, additional_debug_data};
}

}  // namespace

bool operator==(const goaway_payload& lhs, const goaway_payload& rhs) {
  return lhs.m_reserved == rhs.m_reserved &&
         lhs.m_last_stream_id == rhs.m_last_stream_id &&
         lhs.m_error_code == rhs.m_error_code &&
         lhs.m_additional_debug_data == rhs.m_additional_debug_data;
}

bool operator!=(const goaway_payload& lhs, const goaway_payload& rhs) {
  return !(lhs == rhs);
}

goaway_frame::goaway_frame(const goaway_payload& payload)
    : m_header(construct_frame_header(payload)), m_payload(payload) {}

goaway_frame::goaway_frame(const frame_header& fh,
                           const byte_array_t& raw_payload)
    : m_header{fh}, m_payload{construct_payload(raw_payload)} {}

frame_header goaway_frame::get_header() const { return m_header; }

goaway_payload goaway_frame::get_payload() const { return m_payload; }

byte_array_t goaway_frame::serialize() const {
  byte_array_t serialzed_gf = mh2c::serialize(m_header);

  // Serialize Reserved and Last-Stream-ID
  auto last_stream_id =
      extract_low_bit<STREAM_ID_BITS>(m_payload.m_last_stream_id) |
      (m_payload.m_reserved << STREAM_ID_BITS);
  last_stream_id = cvt_host2net(last_stream_id);
  auto begin = reinterpret_cast<const uint8_t*>(&last_stream_id);
  std::copy(begin, begin + sizeof(last_stream_id),
            std::back_inserter(serialzed_gf));

  // Serialize Error Code
  const auto error_code =
      cvt_host2net(static_cast<uint32_t>(m_payload.m_error_code));
  begin = reinterpret_cast<const uint8_t*>(&error_code);
  std::copy(begin, begin + sizeof(error_codes),
            std::back_inserter(serialzed_gf));

  // Serialize Additional Debug Data
  if (m_payload.m_additional_debug_data.size() > 0) {
    const auto& additional_debug_data = m_payload.m_additional_debug_data;
    std::copy(additional_debug_data.begin(), additional_debug_data.end(),
              std::back_inserter(serialzed_gf));
  }

  return serialzed_gf;
}

void goaway_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== GOAWAY FRAME ===\n";
  out_stream << m_header;
  out_stream << "[PAYLOAD]\n";
  out_stream << "  R: " << std::to_string(m_payload.m_reserved) << '\n';
  out_stream << "  Last-Stream-ID: "
             << std::to_string(m_payload.m_last_stream_id) << '\n';
  const auto error_code = m_payload.m_error_code;
  if (error_code < mh2c::error_codes::UNKNOWN_ERROR) {
    out_stream << "  Error Code: " << error_codes_str_map.at(error_code)
               << '\n';
  } else {
    out_stream << "  Error Code: "
               << error_codes_str_map.at(mh2c::error_codes::UNKNOWN_ERROR)
               << "(" << std::to_string(static_cast<uint32_t>(error_code))
               << ")\n";
  }
  out_stream << "  Additional Debug Data: ";
  if (m_payload.m_additional_debug_data.empty() == false) {
    out_stream << m_payload.m_additional_debug_data.data();
  }
  out_stream << '\n';
  return;
}

std::ostream& operator<<(std::ostream& out_stream, const goaway_frame& gf) {
  gf.dump(out_stream);
  return out_stream;
}

bool operator==(const goaway_frame& lhs, const goaway_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_payload == rhs.m_payload;
}

bool operator!=(const goaway_frame& lhs, const goaway_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

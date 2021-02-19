// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/rst_stream_frame.h"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

namespace {

frame_header construct_frame_header(const fh_stream_id_t stream_id) {
  return {static_cast<fh_length_t>(sizeof(error_codes)),
          static_cast<fh_type_t>(frame_type_registry::RST_STREAM), 0u, 0u,
          stream_id};
}

error_codes construct_error_code(const byte_array_t& raw_payload) {
  if (raw_payload.size() < sizeof(error_codes)) {
    const auto msg =
        "invalid payload size: " + std::to_string(raw_payload.size());
    throw std::invalid_argument(msg);
  }

  const auto error_code =
      bytes2integral<std::underlying_type_t<error_codes>>(raw_payload.begin());

  return static_cast<error_codes>(error_code);
}

}  // namespace

rst_stream_frame::rst_stream_frame(const fh_stream_id_t stream_id,
                                   const error_codes error_code)
    : m_header{construct_frame_header(stream_id)}, m_error_code{error_code} {}

rst_stream_frame::rst_stream_frame(const frame_header& fh,
                                   const byte_array_t& raw_payload)
    : m_header{fh}, m_error_code{construct_error_code(raw_payload)} {}

frame_header rst_stream_frame::get_header() const { return m_header; }

error_codes rst_stream_frame::get_payload() const { return m_error_code; }

byte_array_t rst_stream_frame::serialize() const {
  byte_array_t serialized_rsf = mh2c::serialize(m_header);

  const auto error_code = cvt_host2net(
      static_cast<std::underlying_type_t<error_codes>>(m_error_code));
  auto begin = reinterpret_cast<const byte_array_t::value_type*>(&error_code);
  std::copy(begin, begin + sizeof(error_code),
            std::back_inserter(serialized_rsf));

  return serialized_rsf;
}

void rst_stream_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== RST_STREAM FRAME ===\n";
  out_stream << m_header;
  out_stream << "[PAYLOAD]\n";
  if (m_error_code < mh2c::error_codes::UNKNOWN_ERROR) {
    out_stream << "  Error Code: " << error_codes_str_map.at(m_error_code)
               << '\n';
  } else {
    out_stream << "  Error Code: "
               << error_codes_str_map.at(mh2c::error_codes::UNKNOWN_ERROR)
               << "(" << std::to_string(static_cast<uint32_t>(m_error_code))
               << ")\n";
  }
  return;
}

std::ostream& operator<<(std::ostream& out_stream,
                         const rst_stream_frame& rsf) {
  rsf.dump(out_stream);
  return out_stream;
}

bool operator==(const rst_stream_frame& lhs, const rst_stream_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_error_code == rhs.m_error_code;
}

bool operator!=(const rst_stream_frame& lhs, const rst_stream_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

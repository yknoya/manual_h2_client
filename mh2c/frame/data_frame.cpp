// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/data_frame.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/cast.h"

namespace mh2c {

namespace {

template <typename Payload>
frame_header construct_frame_header(fh_flags_t flags, fh_stream_id_t stream_id,
                                    const Payload& payload) {
  return {cast_to_fh_length(payload.size()),
          underlying_cast(frame_type_registry::DATA), flags, 0, stream_id};
}

}  // namespace

using pad_length_t = uint8_t;

data_frame::data_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
                       const byte_array_t& payload)
    : m_header(construct_frame_header(flags, stream_id, payload)),
      m_payload(payload) {}

data_frame::data_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
                       const std::string& payload)
    : m_header(construct_frame_header(flags, stream_id, payload)),
      m_payload(payload.begin(), payload.end()) {}

frame_header data_frame::get_header() const { return m_header; }

byte_array_t data_frame::get_payload() const { return m_payload; }

byte_array_t data_frame::serialize() const {
  byte_array_t serialized_df = mh2c::serialize(m_header);
  std::copy(m_payload.begin(), m_payload.end(),
            std::back_inserter(serialized_df));

  return serialized_df;
}

void data_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== DATA FRAME ===\n";
  out_stream << m_header;
  out_stream << "[PAYLOAD]\n";

  const auto is_padded = is_flag_set(m_header.m_flags, df_flag::PADDED);
  const pad_length_t pad_length = is_padded ? m_payload[0] : 0u;
  const auto data_ptr =
      is_padded ? &m_payload[sizeof(pad_length_t)] : &m_payload[0];
  const auto data_length =
      is_padded ? m_header.m_length - pad_length - sizeof(pad_length_t)
                : m_header.m_length;
  const std::string_view data(reinterpret_cast<const char*>(data_ptr),
                              data_length);

  out_stream << "  Pad Length: " << std::to_string(pad_length) << '\n';
  out_stream << "  Data      : \n";
  out_stream << "    ";
  std::for_each(data.begin(), data.end(), [&out_stream](const char c) {
    out_stream << c;
    if (c == '\n') {
      out_stream << "    ";
    }
  });
  out_stream << '\n';
  out_stream << "  Padding   : \n";

  uint32_t counter{0};
  const auto pad_begin =
      is_padded ? m_payload.end() - pad_length : m_payload.end();
  std::for_each(pad_begin, m_payload.end(),
                [&out_stream, &counter](const auto& elem) {
                  out_stream << std::hex << std::setw(2) << std::setfill('0')
                             << static_cast<int>(elem);
                  if (++counter % 8 == 0) {
                    out_stream << "\n              ";
                  } else {
                    out_stream << ' ';
                  }
                });
  out_stream << '\n';

  return;
}

std::ostream& operator<<(std::ostream& out_stream, const data_frame& sf) {
  sf.dump(out_stream);
  return out_stream;
}

bool operator==(const data_frame& lhs, const data_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_payload == rhs.m_payload;
}

bool operator!=(const data_frame& lhs, const data_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

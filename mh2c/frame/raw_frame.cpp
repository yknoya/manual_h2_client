// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/raw_frame.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <ostream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

raw_frame::raw_frame(const frame_header& header, const byte_array_t& payload)
    : m_header{header}, m_payload{payload} {}

byte_array_t raw_frame::serialize() const {
  byte_array_t serialzed_sf = mh2c::serialize(m_header);
  std::copy(m_payload.begin(), m_payload.end(),
            std::back_inserter(serialzed_sf));
  return serialzed_sf;
}

frame_header raw_frame::get_header() const { return m_header; }

byte_array_t raw_frame::get_payload() const { return m_payload; }

void raw_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== GENERIC FRAME (unimplemented) ===\n"
             << m_header << "[PAYLOAD]\n";

  uint32_t counter{0};
  std::for_each(m_payload.begin(), m_payload.end(),
                [&out_stream, &counter](const auto& elem) {
                  out_stream << std::hex << std::setw(2) << std::setfill('0')
                             << static_cast<int>(elem);
                  if (++counter % 8 == 0) {
                    out_stream << '\n';
                  } else {
                    out_stream << ' ';
                  }
                });
  out_stream << '\n';
}

std::ostream& operator<<(std::ostream& out_stream, const raw_frame& gf) {
  gf.dump(out_stream);
  return out_stream;
}

}  // namespace mh2c

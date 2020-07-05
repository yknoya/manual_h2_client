/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, k-noya
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "mh2c/frame/ping_frame.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <stdexcept>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"

namespace mh2c {

namespace {

constexpr uint8_t OPAQUE_DATA_SIZE{8u};

frame_header construct_frame_header(const fh_flags_t flags) {
  return {OPAQUE_DATA_SIZE, static_cast<fh_type_t>(frame_type_registry::PING),
          flags, 0u, 0u};
}

byte_array_t construct_opaque_data(const byte_array_t& opaque_data) {
  const auto opaque_data_size = opaque_data.size();
  if (opaque_data_size > OPAQUE_DATA_SIZE) {
    const auto msg =
        "invalid opaque data size: " + std::to_string(opaque_data_size);
    throw std::invalid_argument(msg);
  }

  byte_array_t constructed_opaque_data(OPAQUE_DATA_SIZE, 0u);
  std::copy(opaque_data.begin(), opaque_data.end(),
            constructed_opaque_data.begin());

  return constructed_opaque_data;
}

}  // namespace

ping_frame::ping_frame(const fh_flags_t flags, const byte_array_t& opaque_data)
    : m_header{construct_frame_header(flags)},
      m_opaque_data{construct_opaque_data(opaque_data)} {}

ping_frame::ping_frame(const frame_header& fh, const byte_array_t& raw_payload)
    : m_header{fh}, m_opaque_data{raw_payload} {}

frame_header ping_frame::get_header() const { return m_header; }

byte_array_t ping_frame::get_payload() const { return m_opaque_data; }

byte_array_t ping_frame::serialize() const {
  byte_array_t serialized_pf = mh2c::serialize(m_header);

  std::copy(m_opaque_data.begin(), m_opaque_data.end(),
            std::back_inserter(serialized_pf));

  return serialized_pf;
}

void ping_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== PING_STREAM FRAME ===\n";
  out_stream << m_header;
  out_stream << "[PAYLOAD]\n";
  uint32_t counter{0};
  std::for_each(m_opaque_data.begin(), m_opaque_data.end(),
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
  return;
}

std::ostream& operator<<(std::ostream& out_stream, const ping_frame& pf) {
  pf.dump(out_stream);
  return out_stream;
}

bool operator==(const ping_frame& lhs, const ping_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_opaque_data == rhs.m_opaque_data;
}

bool operator!=(const ping_frame& lhs, const ping_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

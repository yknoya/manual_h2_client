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
  std::copy(m_payload.begin(), m_payload.end(), std::back_inserter(serialzed_sf));
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

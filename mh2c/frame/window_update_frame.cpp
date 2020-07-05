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
#include "mh2c/frame/window_update_frame.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/util/bit_operation.h"
#include "mh2c/util/byte_order.h"

namespace mh2c {

namespace {

constexpr window_size_t MAX_WINDOW_SIZE{0x7fffffff};
constexpr uint8_t WINDOW_SIZE_BITS{31u};

frame_header construct_frame_header(const fh_stream_id_t stream_id) {
  return {4u, static_cast<fh_type_t>(frame_type_registry::WINDOW_UPDATE), 0u, 0u,
          stream_id};
}

reserved_t construct_reserved(const byte_array_t& raw_payload) {
  if (raw_payload.empty()) {
    const auto msg =
        "invalid payload size: " + std::to_string(raw_payload.size());
    throw std::invalid_argument(msg);
  }

  return extract_high_bit<RESERVED_BITS>(raw_payload[0]);
}

window_size_t construct_window_size(const byte_array_t& raw_payload) {
  const auto payload_size = raw_payload.size();
  if (payload_size != sizeof(window_size_t)) {
    const auto msg = "invalid payload size: " + std::to_string(payload_size);
    throw std::invalid_argument(msg);
  }

  return extract_low_bit<WINDOW_SIZE_BITS>(
      bytes2integral<window_size_t>(raw_payload.begin()));
}

void validate_window_size(const window_size_t window_size) {
  if (window_size > MAX_WINDOW_SIZE) {
    const auto msg = "invalid window size: " + std::to_string(window_size);
    throw std::invalid_argument(msg);
  }
}

}  // namespace

window_update_frame::window_update_frame(
    const fh_stream_id_t stream_id, const window_size_t window_size_increment)
    : window_update_frame{stream_id, 0u, window_size_increment} {}

window_update_frame::window_update_frame(
    const fh_stream_id_t stream_id, const reserved_t reserved,
    const window_size_t window_size_increment)
    : m_header{construct_frame_header(stream_id)},
      m_reserved{reserved},
      m_window_size_increment{window_size_increment} {
  validate_window_size(m_window_size_increment);
}

window_update_frame::window_update_frame(const frame_header& fh,
                                         const byte_array_t& raw_payload)
    : m_header{fh},
      m_reserved{construct_reserved(raw_payload)},
      m_window_size_increment{construct_window_size(raw_payload)} {
  validate_window_size(m_window_size_increment);
}

frame_header window_update_frame::get_header() const { return m_header; }

window_size_t window_update_frame::get_payload() const {
  return (extract_low_bit<RESERVED_BITS>(m_reserved) << WINDOW_SIZE_BITS) |
         extract_low_bit<WINDOW_SIZE_BITS>(m_window_size_increment);
}

byte_array_t window_update_frame::serialize() const {
  byte_array_t serialized_wuf = mh2c::serialize(m_header);

  decltype(m_window_size_increment) payload = cvt_host2net(
      (extract_low_bit<RESERVED_BITS>(m_reserved) << WINDOW_SIZE_BITS) |
      extract_low_bit<WINDOW_SIZE_BITS>(m_window_size_increment));

  const auto begin = reinterpret_cast<uint8_t*>(&payload);
  std::copy(begin, begin + sizeof(m_window_size_increment),
            std::back_inserter(serialized_wuf));

  return serialized_wuf;
}

void window_update_frame::dump(std::ostream& out_stream) const {
  out_stream << "=== WINDOW_UPDATE FRAME ===\n";
  out_stream << m_header;
  out_stream << "[PAYLOAD]\n";
  out_stream << "  R: " << std::to_string(m_reserved) << '\n';
  out_stream << "  Window size Increment: "
             << std::to_string(m_window_size_increment) << '\n';
  return;
}

std::ostream& operator<<(std::ostream& out_stream,
                         const window_update_frame& wuf) {
  wuf.dump(out_stream);
  return out_stream;
}

bool operator==(const window_update_frame& lhs, const window_update_frame& rhs) {
  return lhs.m_header == rhs.m_header && lhs.m_reserved == rhs.m_reserved &&
         lhs.m_window_size_increment == rhs.m_window_size_increment;
}

bool operator!=(const window_update_frame& lhs, const window_update_frame& rhs) {
  return !(lhs == rhs);
}

}  // namespace mh2c

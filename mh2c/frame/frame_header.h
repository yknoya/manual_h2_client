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
#ifndef MH2C_FRAME_FRAME_HEADER_H_
#define MH2C_FRAME_FRAME_HEADER_H_

#include <cstdint>
#include <ostream>

#include "mh2c/common/byte_array.h"

namespace mh2c {

/*
 * [Frame Fromat]
 * +-----------------------------------------------+
 * |                 Length (24)                   |
 * +---------------+---------------+---------------+
 * |   Type (8)    |   Flags (8)   |
 * +-+-+-----------+---------------+-------------------------------+
 * |R|                 Stream Identifier (31)                      |
 * +=+=============================================================+
 * |                   Frame Payload (0...)                      ...
 * +---------------------------------------------------------------+
 * cf. https://tools.ietf.org/html/rfc7540#section-4.1
 */
using fh_length_t = uint32_t;
using fh_type_t = uint8_t;
using fh_flags_t = uint8_t;
using fh_reserved_t = uint8_t;
using fh_stream_id_t = uint32_t;

constexpr uint8_t FRAME_HEADER_BYTES{9u};
constexpr uint8_t LENGTH_BITS{24u};
constexpr uint8_t RESERVED_BITS{1u};
constexpr uint8_t STREAM_ID_BITS{31u};
constexpr fh_stream_id_t MAX_STREAM_ID{0x7fffffff};

struct frame_header {
  fh_length_t m_length;  // Actually 24 bit
  fh_type_t m_type;
  fh_flags_t m_flags;
  fh_reserved_t m_reserved;    // Actually 1 bit
  fh_stream_id_t m_stream_id;  // Actually 31 bit
};

byte_array_t serialize(const frame_header& fh);
frame_header build_frame_header(const byte_array_t& raw_data);

std::ostream& operator<<(std::ostream& out_stream, const frame_header& fh);
bool operator==(const frame_header& lhs, const frame_header& rhs);
bool operator!=(const frame_header& lhs, const frame_header& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_FRAME_HEADER_H_

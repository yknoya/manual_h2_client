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
#ifndef MH2C_FRAME_GOAWAY_FRAME_H_
#define MH2C_FRAME_GOAWAY_FRAME_H_

#include <cstdint>
#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.8
struct goaway_payload {
  uint8_t m_reserved;               // Actually 1 bit
  fh_stream_id_t m_last_stream_id;  // Actually 31 bit
  error_codes m_error_code;
  byte_array_t m_additional_debug_data;
};

bool operator==(const goaway_payload& lhs, const goaway_payload& rhs);
bool operator!=(const goaway_payload& lhs, const goaway_payload& rhs);

class goaway_frame : public i_frame<frame_header> {
 public:
  explicit goaway_frame(const goaway_payload& payload);
  goaway_frame(const frame_header& fh, const byte_array_t& raw_payload);

  frame_header get_header() const override;
  goaway_payload get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  goaway_payload m_payload;

  friend bool operator==(const goaway_frame& lhs, const goaway_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const goaway_frame& gf);
bool operator==(const goaway_frame& lhs, const goaway_frame& rhs);
bool operator!=(const goaway_frame& lhs, const goaway_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_GOAWAY_FRAME_H_

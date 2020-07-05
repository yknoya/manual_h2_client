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
#ifndef MH2C_FRAME_CONTINUATION_FRAME_H_
#define MH2C_FRAME_CONTINUATION_FRAME_H_

#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-6.10
enum class cf_flag : fh_flags_t {
  END_HEADERS = 0x04,
};

class continuation_frame : public i_frame<frame_header> {
 public:
  continuation_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
                     const header_block_t& header_block,
                     const header_encode_mode mode,
                     const dynamic_table& dynamic_table);
  continuation_frame(const frame_header& fh, const byte_array_t& raw_payload,
                     const dynamic_table& dynamic_table);

  frame_header get_header() const override;
  header_block_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  byte_array_t m_encoded_payload;
  frame_header m_header;
  header_block_t m_header_block;

  friend bool operator==(const continuation_frame& lhs,
                         const continuation_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream, const continuation_frame& hf);
bool operator==(const continuation_frame& lhs, const continuation_frame& rhs);
bool operator!=(const continuation_frame& lhs, const continuation_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_CONTINUATION_FRAME_H_

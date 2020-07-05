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
#ifndef MH2C_FRAME_SETTINGS_FRAME_H_
#define MH2C_FRAME_SETTINGS_FRAME_H_

#include <cstdint>
#include <map>
#include <ostream>
#include <utility>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

enum class sf_flag : fh_flags_t {
  ACK = 0x01,
};

// cf. https://tools.ietf.org/html/rfc7540#section-6.5.1
using sf_id_t = uint16_t;
using sf_value_t = uint32_t;
using sf_pair_t = std::pair<sf_id_t, sf_value_t>;
using sf_payload_t = std::map<sf_id_t, sf_value_t>;

// cf. https://tools.ietf.org/html/rfc7540#section-6.5.2
enum class sf_parameter : sf_id_t {
  SETTINGS_HEADER_TABLE_SIZE = 0x01,
  SETTINGS_ENABLE_PUSH = 0x02,
  SETTINGS_MAX_CONCURRENT_STREAM = 0x03,
  SETTINGS_INITIAL_WINDOW_SIZE = 0x04,
  SETTINGS_MAX_FRAME_SIZE = 0x05,
  SETTINGS_MAX_HEADER_LIST_SIZE = 0x06,
  SETTINGS_UNKNOWN = 0x07,
};

class settings_frame : public i_frame<frame_header> {
 public:
  settings_frame(const fh_flags_t flags, const fh_stream_id_t stream_id,
                 const sf_payload_t& payload);

  frame_header get_header() const override;
  sf_payload_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  sf_payload_t m_payload;

  friend bool operator==(const settings_frame& lhs, const settings_frame& rhs);
};

sf_pair_t make_sf_parameter(const sf_parameter id, const sf_value_t value);
std::ostream& operator<<(std::ostream& out_stream, const settings_frame& sf);
bool operator==(const settings_frame& lhs, const settings_frame& rhs);
bool operator!=(const settings_frame& lhs, const settings_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_SETTINGS_FRAME_H_

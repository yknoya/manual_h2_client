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

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(ping_frame_test, serialize) {
  const mh2c::byte_array_t opaque_data{'p', 'i', 'n', 'g'};
  const mh2c::ping_frame pf(0u, opaque_data);

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x08,        // length
      0x06,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x70, 0x69, 0x6e, 0x67,  // Opaque Data
      0x00, 0x00, 0x00, 0x00,  // Opaque Data
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(ping_frame_test, serialize_with_ack) {
  const mh2c::byte_array_t opaque_data{'p', 'i', 'n', 'g'};
  const mh2c::ping_frame pf{static_cast<mh2c::fh_flags_t>(mh2c::pf_flag::ACK),
                            opaque_data};

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x08,        // length
      0x06,                    // type
      0x01,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x70, 0x69, 0x6e, 0x67,  // Opaque Data
      0x00, 0x00, 0x00, 0x00,  // Opaque Data
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(ping_frame_test, throw_if_opaque_data_size_is_over) {
  const mh2c::byte_array_t opaque_data{'t', 'h', 'i', 's', ' ', 'i', 's', ' ',
                                       't', 'o', 'o', ' ', 'l', 'o', 'n', 'g'};
  EXPECT_ANY_THROW(mh2c::ping_frame(0u, opaque_data));
}

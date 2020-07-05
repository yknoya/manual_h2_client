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
#include "mh2c/frame/goaway_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"

TEST(goaway_frame_test, serialize) {
  const mh2c::goaway_payload gf_payload{
      0u,                           // Reserved
      0u,                           // Last-Stream-ID
      mh2c::error_codes::NO_ERROR,  // Error Code
      {},                           // Additional Debug Data
  };
  const mh2c::goaway_frame gf(gf_payload);

  const mh2c::byte_array_t expected_raw_gf{
      0x00, 0x00, 0x08,        // length
      0x07,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x00, 0x00, 0x00, 0x00,  // Reserved and Last-Stream-ID
      0x00, 0x00, 0x00, 0x00,  // Error Code
  };

  const auto serialized_gf = gf.serialize();
  EXPECT_EQ(expected_raw_gf, serialized_gf);
}

TEST(goaway_frame_test, serialize_with_additional_debug_data) {
  const mh2c::goaway_payload gf_payload{
      0u,                         // Reserved
      128u,                       // Last-Stream-ID
      mh2c::error_codes::CANCEL,  // Error Code
      {'t', 'e', 's', 't'},       // Additional Debug Data
  };
  const mh2c::goaway_frame gf(gf_payload);

  const mh2c::byte_array_t expected_raw_gf{
      0x00, 0x00, 0x0c,        // length
      0x07,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x00, 0x00, 0x00, 0x80,  // Reserved and Last-Stream-ID
      0x00, 0x00, 0x00, 0x08,  // Error Code
      0x74, 0x65, 0x73, 0x74,  // Additional Debug Data
  };

  const auto serialized_gf = gf.serialize();
  EXPECT_EQ(expected_raw_gf, serialized_gf);
}

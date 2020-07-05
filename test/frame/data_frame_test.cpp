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
#include "mh2c/frame/data_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(data_frame_test, serialize) {
  const mh2c::data_frame df{0x00, 0x01, "test"};

  const mh2c::byte_array_t expected_raw_df{
      0x00, 0x00, 0x04,        // length
      0x00,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x74, 0x65, 0x73, 0x74,  // Data
  };

  const auto serialized_df = df.serialize();
  EXPECT_EQ(expected_raw_df, serialized_df);
}

TEST(data_frame_test, serialize_with_padding) {
  const mh2c::data_frame df{
      static_cast<mh2c::fh_flags_t>(mh2c::df_flag::PADDED), 0x01,
      mh2c::byte_array_t{0x02, 0x74, 0x65, 0x73, 0x74, 0xff, 0xff}};

  const mh2c::byte_array_t expected_raw_df{
      0x00, 0x00, 0x07,        // length
      0x00,                    // type
      0x08,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x02,                    // Pad Length
      0x74, 0x65, 0x73, 0x74,  // Data
      0xff, 0xff,              // Padding
  };

  const auto serialized_df = df.serialize();
  EXPECT_EQ(expected_raw_df, serialized_df);
}

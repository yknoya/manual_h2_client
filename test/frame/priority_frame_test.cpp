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
#include "mh2c/frame/priority_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(priority_frame_test, serialize) {
  const mh2c::priority_payload payload{0u, 1u, 0u};
  const mh2c::priority_frame pf(3u, payload);

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x05,        // length
      0x02,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x03,  // reserved and stream id
      0x00, 0x00, 0x00, 0x01,  // Exclusive and Stream Dependency
      0x00,                    // Weight
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(priority_frame_test, serialize_with_exclusive) {
  const mh2c::priority_payload payload{1u, 1u, 0u};
  const mh2c::priority_frame pf(3u, payload);

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x05,        // length
      0x02,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x03,  // reserved and stream id
      0x80, 0x00, 0x00, 0x01,  // Exclusive and Stream Dependency
      0x00,                    // Weight
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(priority_frame_test, serialize_with_weight) {
  const mh2c::priority_payload payload{0u, 1u, 255u};
  const mh2c::priority_frame pf(3u, payload);

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x05,        // length
      0x02,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x03,  // reserved and stream id
      0x00, 0x00, 0x00, 0x01,  // Exclusive and Stream Dependency
      0xff,                    // Weight
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(priority_frame_test, throw_if_exclusive_is_invalid) {
  const mh2c::priority_payload payload{2u, 1u, 0u};
  EXPECT_ANY_THROW(mh2c::priority_frame(3u, payload));
}

TEST(priority_frame_test, throw_if_stream_dependency_is_invalid) {
  const mh2c::priority_payload payload{0u, 0x80000000, 0u};
  EXPECT_ANY_THROW(mh2c::priority_frame(3u, payload));
}

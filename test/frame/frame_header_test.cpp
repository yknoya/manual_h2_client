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
#include "mh2c/frame/frame_header.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(frame_header_test, serialize) {
  const mh2c::frame_header fh{
      0x40,  // length
      0x04,  // type
      0x02,  // falgs
      0x00,  // reserved
      0x80   // stream_id
  };
  const mh2c::byte_array_t expected_raw_fh{
      0x00, 0x00, 0x40,       // length
      0x04,                   // type
      0x02,                   // flags
      0x00, 0x00, 0x00, 0x80  // reserved and stream id
  };

  const auto serialized_fh = mh2c::serialize(fh);
  EXPECT_EQ(expected_raw_fh, serialized_fh);
}

TEST(frame_header_test, serialize_set_reserved) {
  const mh2c::frame_header fh{
      0x1234,  // length
      0x05,    // type
      0x08,    // falgs
      0x01,    // reserved
      0xffff   // stream_id
  };
  const mh2c::byte_array_t expected_raw_fh{
      0x00, 0x12, 0x34,       // length
      0x05,                   // type
      0x08,                   // flags
      0x80, 0x00, 0xff, 0xff  // reserved and stream id
  };

  const auto serialized_fh = mh2c::serialize(fh);
  EXPECT_EQ(expected_raw_fh, serialized_fh);
}

TEST(frame_header_test, serialize_max) {
  const mh2c::frame_header fh{
      0xffffff,   // length
      0xff,       // type
      0xff,       // falgs
      0x01,       // reserved
      0x7fffffff  // stream_id
  };
  const mh2c::byte_array_t expected_raw_fh{
      0xff, 0xff, 0xff,       // length
      0xff,                   // type
      0xff,                   // flags
      0xff, 0xff, 0xff, 0xff  // reserved and stream id
  };

  const auto serialized_fh = mh2c::serialize(fh);
  EXPECT_EQ(expected_raw_fh, serialized_fh);
}

TEST(frame_header_test, build_frame_header) {
  const mh2c::byte_array_t raw_fh{
      0x00, 0x00, 0x40,       // length
      0x04,                   // type
      0x02,                   // flags
      0x00, 0x00, 0x00, 0x80  // reserved and stream id
  };

  const mh2c::frame_header expected_fh{
      0x40,  // length
      0x04,  // type
      0x02,  // falgs
      0x00,  // reserved
      0x80   // stream_id
  };

  const auto fh = mh2c::build_frame_header(raw_fh);
  EXPECT_EQ(expected_fh, fh);
}

TEST(frame_header_test, build_frame_header_set_reserved) {
  const mh2c::byte_array_t raw_fh{
      0x00, 0x12, 0x34,       // length
      0x05,                   // type
      0x08,                   // flags
      0x80, 0x00, 0xff, 0xff  // reserved and stream id
  };

  const mh2c::frame_header expected_fh{
      0x1234,  // length
      0x05,    // type
      0x08,    // falgs
      0x01,    // reserved
      0xffff   // stream_id
  };

  const auto fh = mh2c::build_frame_header(raw_fh);
  EXPECT_EQ(expected_fh, fh);
}

TEST(frame_header_test, build_frame_header_max) {
  const mh2c::byte_array_t raw_fh{
      0xff, 0xff, 0xff,       // length
      0xff,                   // type
      0xff,                   // flags
      0xff, 0xff, 0xff, 0xff  // reserved and stream id
  };

  const mh2c::frame_header expected_fh{
      0xffffff,   // length
      0xff,       // type
      0xff,       // falgs
      0x01,       // reserved
      0x7fffffff  // stream_id
  };

  const auto fh = mh2c::build_frame_header(raw_fh);
  EXPECT_EQ(expected_fh, fh);
}

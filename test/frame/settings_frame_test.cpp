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
#include "mh2c/frame/settings_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(settings_frame_test, serialize) {
  const mh2c::fh_flags_t flags = 0u;
  const mh2c::fh_stream_id_t stream_id = 0u;
  const mh2c::sf_payload_t sf_payload{
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE,
                              512u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_ENABLE_PUSH, 0u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_MAX_CONCURRENT_STREAM,
                              16u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE,
                              65535u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_MAX_FRAME_SIZE,
                              65535u),
      mh2c::make_sf_parameter(mh2c::sf_parameter::SETTINGS_MAX_HEADER_LIST_SIZE,
                              256u)};
  const mh2c::settings_frame sf(flags, stream_id, sf_payload);

  const mh2c::byte_array_t expected_raw_sf{
      0x00, 0x00, 0x24,        // length
      0x04,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x00, 0x01,              // SETTINGS_HEADER_TABLE_SIZE
      0x00, 0x00, 0x02, 0x00,  // 512
      0x00, 0x02,              // SETTINGS_ENABLE_PUSH
      0x00, 0x00, 0x00, 0x00,  // 0
      0x00, 0x03,              // SETTINGS_MAX_CONCURRENT_STREAM
      0x00, 0x00, 0x00, 0x10,  // 16
      0x00, 0x04,              // SETTINGS_INITIAL_WINDOW_SIZE
      0x00, 0x00, 0xff, 0xff,  // 65535
      0x00, 0x05,              // SETTINGS_MAX_FRAME_SIZE
      0x00, 0x00, 0xff, 0xff,  // 65535
      0x00, 0x06,              // SETTINGS_MAX_HEADER_LIST_SIZE
      0x00, 0x00, 0x01, 0x00,  // 256
  };

  const auto serialized_sf = sf.serialize();
  EXPECT_EQ(expected_raw_sf, serialized_sf);
}

TEST(settings_frame_test, serialize_ack) {
  const mh2c::fh_flags_t flags = 1u;
  const mh2c::fh_stream_id_t stream_id = 0u;
  const mh2c::sf_payload_t sf_payload{};
  const mh2c::settings_frame sf(flags, stream_id, sf_payload);

  const mh2c::byte_array_t expected_raw_sf{
      0x00, 0x00, 0x00,        // length
      0x04,                    // type
      0x01,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
  };

  const auto serialized_sf = sf.serialize();
  EXPECT_EQ(expected_raw_sf, serialized_sf);
}

// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/settings_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(settings_frame_test, serialize) {
  const mh2c::fh_flags_t flags = 0u;
  const mh2c::fh_stream_id_t stream_id = 0u;
  const mh2c::sf_payload_t sf_payload{mh2c::make_sf_payload(
      {{mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE, 512u},
       {mh2c::sf_parameter::SETTINGS_ENABLE_PUSH, 0u},
       {mh2c::sf_parameter::SETTINGS_MAX_CONCURRENT_STREAM, 16u},
       {mh2c::sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE, 65535u},
       {mh2c::sf_parameter::SETTINGS_MAX_FRAME_SIZE, 65535u},
       {mh2c::sf_parameter::SETTINGS_MAX_HEADER_LIST_SIZE, 256u}})};
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

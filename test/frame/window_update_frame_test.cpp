// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/window_update_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"

TEST(window_update_frame_test, serialize) {
  const mh2c::window_update_frame wuf(0u, 0x01234567);

  const mh2c::byte_array_t expected_raw_wuf{
      0x00, 0x00, 0x04,        // length
      0x08,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x01, 0x23, 0x45, 0x67,  // Reserved and Window Size Increment;
  };

  const auto serialized_wuf = wuf.serialize();
  EXPECT_EQ(expected_raw_wuf, serialized_wuf);
}

TEST(window_update_frame_test, serialize_with_specific_stream) {
  const mh2c::window_update_frame wuf(1u, 0x01234567);

  const mh2c::byte_array_t expected_raw_wuf{
      0x00, 0x00, 0x04,        // length
      0x08,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x01, 0x23, 0x45, 0x67,  // Reserved and Window Size Increment;
  };

  const auto serialized_wuf = wuf.serialize();
  EXPECT_EQ(expected_raw_wuf, serialized_wuf);
}

TEST(window_update_frame_test, serialize_with_reserved) {
  const mh2c::window_update_frame wuf(0u, 1u, 0x01234567);

  const mh2c::byte_array_t expected_raw_wuf{
      0x00, 0x00, 0x04,        // length
      0x08,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x81, 0x23, 0x45, 0x67,  // Reserved and Window Size Increment;
  };

  const auto serialized_wuf = wuf.serialize();
  EXPECT_EQ(expected_raw_wuf, serialized_wuf);
}

TEST(window_update_frame_test, throw_if_window_size_is_over) {
  EXPECT_ANY_THROW(mh2c::window_update_frame(0u, 0x80000000));
}

TEST(window_update_frame_test, parse_raw_payload_with_reserved_bit) {
  const mh2c::frame_header header{
      4u, 0x08u, 0u, 0u, 3u,
  };
  const mh2c::byte_array_t raw_payload{0x81, 0x23, 0x45, 0x67};
  const mh2c::window_update_frame frame{header, raw_payload};
  const auto serialized = frame.serialize();

  EXPECT_EQ(header, frame.get_header());
  EXPECT_EQ(0x81234567u, frame.get_payload());
  EXPECT_EQ(raw_payload,
            mh2c::byte_array_t(serialized.begin() + 9, serialized.end()));
}

TEST(window_update_frame_test, throw_if_payload_size_is_zero) {
  const mh2c::frame_header header{
      0u, 0x08u, 0u, 0u, 0u,
  };

  EXPECT_ANY_THROW(mh2c::window_update_frame(header, {}));
}

TEST(window_update_frame_test, throw_if_payload_size_is_three) {
  const mh2c::frame_header header{
      3u, 0x08u, 0u, 0u, 0u,
  };

  EXPECT_ANY_THROW(mh2c::window_update_frame(header, {0x01, 0x23, 0x45}));
}

TEST(window_update_frame_test, throw_if_payload_size_is_five) {
  const mh2c::frame_header header{
      5u, 0x08u, 0u, 0u, 0u,
  };

  EXPECT_ANY_THROW(
      mh2c::window_update_frame(header, {0x01, 0x23, 0x45, 0x67, 0x89}));
}

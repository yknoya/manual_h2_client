// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/window_update_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

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

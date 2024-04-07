// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
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

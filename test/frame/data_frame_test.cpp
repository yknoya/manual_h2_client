// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/data_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"

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
      mh2c::make_frame_header_flags(mh2c::df_flag::PADDED), 0x01,
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

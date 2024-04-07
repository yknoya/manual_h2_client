// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/rst_stream_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"

TEST(rst_stream_frame_test, serialize) {
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::rst_stream_frame rsf{stream_id, mh2c::error_codes::NO_ERROR};

  const mh2c::byte_array_t expected_raw_rsf{
      0x00, 0x00, 0x04,        // length
      0x03,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x00,  // Error Code
  };

  const auto serialized_rsf = rsf.serialize();
  EXPECT_EQ(expected_raw_rsf, serialized_rsf);
}

TEST(rst_stream_frame_test, serialize_unknown_error_code) {
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::rst_stream_frame rsf{stream_id, mh2c::error_codes::UNKNOWN_ERROR};

  const mh2c::byte_array_t expected_raw_rsf{
      0x00, 0x00, 0x04,        // length
      0x03,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x0e,  // Error Code
  };

  const auto serialized_rsf = rsf.serialize();
  EXPECT_EQ(expected_raw_rsf, serialized_rsf);
}

// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/goaway_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/error_codes.h"

TEST(goaway_frame_test, serialize) {
  const mh2c::goaway_payload gf_payload{
      0u,                           // Reserved
      0u,                           // Last-Stream-ID
      mh2c::error_codes::NO_ERROR,  // Error Code
      {},                           // Additional Debug Data
  };
  const mh2c::goaway_frame gf(gf_payload);

  const mh2c::byte_array_t expected_raw_gf{
      0x00, 0x00, 0x08,        // length
      0x07,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x00, 0x00, 0x00, 0x00,  // Reserved and Last-Stream-ID
      0x00, 0x00, 0x00, 0x00,  // Error Code
  };

  const auto serialized_gf = gf.serialize();
  EXPECT_EQ(expected_raw_gf, serialized_gf);
}

TEST(goaway_frame_test, serialize_with_additional_debug_data) {
  const mh2c::goaway_payload gf_payload{
      0u,                         // Reserved
      128u,                       // Last-Stream-ID
      mh2c::error_codes::CANCEL,  // Error Code
      {'t', 'e', 's', 't'},       // Additional Debug Data
  };
  const mh2c::goaway_frame gf(gf_payload);

  const mh2c::byte_array_t expected_raw_gf{
      0x00, 0x00, 0x0c,        // length
      0x07,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x00, 0x00, 0x00, 0x80,  // Reserved and Last-Stream-ID
      0x00, 0x00, 0x00, 0x08,  // Error Code
      0x74, 0x65, 0x73, 0x74,  // Additional Debug Data
  };

  const auto serialized_gf = gf.serialize();
  EXPECT_EQ(expected_raw_gf, serialized_gf);
}

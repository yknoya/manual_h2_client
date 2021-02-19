// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/ping_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

TEST(ping_frame_test, serialize) {
  const mh2c::byte_array_t opaque_data{'p', 'i', 'n', 'g'};
  const mh2c::ping_frame pf(0u, opaque_data);

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x08,        // length
      0x06,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x70, 0x69, 0x6e, 0x67,  // Opaque Data
      0x00, 0x00, 0x00, 0x00,  // Opaque Data
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(ping_frame_test, serialize_with_ack) {
  const mh2c::byte_array_t opaque_data{'p', 'i', 'n', 'g'};
  const mh2c::ping_frame pf{static_cast<mh2c::fh_flags_t>(mh2c::pf_flag::ACK),
                            opaque_data};

  const mh2c::byte_array_t expected_raw_pf{
      0x00, 0x00, 0x08,        // length
      0x06,                    // type
      0x01,                    // flags
      0x00, 0x00, 0x00, 0x00,  // reserved and stream id
      0x70, 0x69, 0x6e, 0x67,  // Opaque Data
      0x00, 0x00, 0x00, 0x00,  // Opaque Data
  };

  const auto serialized_pf = pf.serialize();
  EXPECT_EQ(expected_raw_pf, serialized_pf);
}

TEST(ping_frame_test, throw_if_opaque_data_size_is_over) {
  const mh2c::byte_array_t opaque_data{'t', 'h', 'i', 's', ' ', 'i', 's', ' ',
                                       't', 'o', 'o', ' ', 'l', 'o', 'n', 'g'};
  EXPECT_ANY_THROW(mh2c::ping_frame(0u, opaque_data));
}

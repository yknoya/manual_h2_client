// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/raw_frame.h"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"

TEST(raw_frame_test, serialize_concatenates_header_and_payload) {
  const mh2c::frame_header header{
      3u, 0x0f, 0x01, 0u, 1u,
  };
  const mh2c::byte_array_t payload{0x01, 0x23, 0xab};
  const mh2c::raw_frame frame{header, payload};
  const mh2c::byte_array_t expected{
      0x00, 0x00, 0x03, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x23, 0xab,
  };

  EXPECT_EQ(header, frame.get_header());
  EXPECT_EQ(payload, frame.get_payload());
  EXPECT_EQ(expected, frame.serialize());
}

TEST(raw_frame_test, dump_prints_generic_frame_and_hex_payload) {
  const mh2c::frame_header header{
      3u, 0x0f, 0x01, 0u, 1u,
  };
  const mh2c::raw_frame frame{header, {0x01, 0x23, 0xab}};

  std::ostringstream oss;
  frame.dump(oss);

  const auto dumped = oss.str();
  EXPECT_NE(std::string::npos,
            dumped.find("=== GENERIC FRAME (unimplemented) ==="));
  EXPECT_NE(std::string::npos, dumped.find("[PAYLOAD]"));
  EXPECT_NE(std::string::npos, dumped.find("01 23 ab"));
}

TEST(raw_frame_test, output_operator_matches_dump_output) {
  const mh2c::frame_header header{
      2u, 0x0f, 0x00, 0u, 3u,
  };
  const mh2c::raw_frame frame{header, {0xde, 0xad}};

  std::ostringstream expected;
  std::ostringstream actual;
  frame.dump(expected);
  actual << frame;

  EXPECT_EQ(expected.str(), actual.str());
}

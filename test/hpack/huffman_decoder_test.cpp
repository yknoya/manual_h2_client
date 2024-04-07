// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/huffman_decoder.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/header_encoder.h"
#include "mh2c/hpack/header_type.h"

TEST(huffman_decoder, simple_data_302) {
  const mh2c::byte_array_t encoded_data{0x64, 0x02};
  // expected_data: 302
  const mh2c::byte_array_t expected_data{0x33, 0x30, 0x32};
  const auto decoded_data = mh2c::huffman::decode(encoded_data);
  EXPECT_EQ(expected_data, decoded_data);
}

TEST(huffman_decoder, simple_data_private) {
  const mh2c::byte_array_t encoded_data{0xae, 0xc3, 0x77, 0x1a, 0x4b};
  // expected_data: private
  const mh2c::byte_array_t expected_data{0x70, 0x72, 0x69, 0x76,
                                         0x61, 0x74, 0x65};
  const auto decoded_data = mh2c::huffman::decode(encoded_data);
  EXPECT_EQ(expected_data, decoded_data);
}

TEST(huffman_decoder, simple_data_date) {
  const mh2c::byte_array_t encoded_data{
      0xd0, 0x7a, 0xbe, 0x94, 0x10, 0x54, 0xd4, 0x44, 0xa8, 0x20, 0x05,
      0x95, 0x04, 0x0b, 0x81, 0x66, 0xe0, 0x82, 0xa6, 0x2d, 0x1b, 0xff};
  // expected_data: Mon, 21 Oct 2013 20:13:21 GMT
  const mh2c::byte_array_t expected_data{
      0x4d, 0x6f, 0x6e, 0x2c, 0x20, 0x32, 0x31, 0x20, 0x4f, 0x63,
      0x74, 0x20, 0x32, 0x30, 0x31, 0x33, 0x20, 0x32, 0x30, 0x3a,
      0x31, 0x33, 0x3a, 0x32, 0x31, 0x20, 0x47, 0x4d, 0x54};
  const auto decoded_data = mh2c::huffman::decode(encoded_data);
  EXPECT_EQ(expected_data, decoded_data);
}

TEST(huffman_decoder, simple_data_url) {
  const mh2c::byte_array_t encoded_data{0x9d, 0x29, 0xad, 0x17, 0x18, 0x63,
                                        0xc7, 0x8f, 0x0b, 0x97, 0xc8, 0xe9,
                                        0xae, 0x82, 0xae, 0x43, 0xd3};
  // expectead_data: https://www.example.com
  const mh2c::byte_array_t expected_data{
      0x68, 0x74, 0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e,
      0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d};
  const auto decoded_data = mh2c::huffman::decode(encoded_data);
  EXPECT_EQ(expected_data, decoded_data);
}

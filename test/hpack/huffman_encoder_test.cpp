/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, k-noya
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "mh2c/hpack/huffman_encoder.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/header_encoder.h"
#include "mh2c/hpack/header_type.h"

TEST(huffman_encoder, simple_data_302) {
  // data: 302
  const mh2c::byte_array_t data{0x33, 0x30, 0x32};
  const mh2c::byte_array_t expected_encoded_data{0x64, 0x02};
  const auto encoded_data = mh2c::huffman::encode(data);
  EXPECT_EQ(expected_encoded_data, encoded_data);
}

TEST(huffman_encoder, simple_data_private) {
  // data: private
  const mh2c::byte_array_t data{0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65};
  const mh2c::byte_array_t expected_encoded_data{0xae, 0xc3, 0x77, 0x1a, 0x4b};
  const auto encoded_data = mh2c::huffman::encode(data);
  EXPECT_EQ(expected_encoded_data, encoded_data);
}

TEST(huffman_encoder, simple_data_date) {
  // expected_encoded_data: Mon, 21 Oct 2013 20:13:21 GMT
  const mh2c::byte_array_t data{0x4d, 0x6f, 0x6e, 0x2c, 0x20, 0x32, 0x31, 0x20,
                                0x4f, 0x63, 0x74, 0x20, 0x32, 0x30, 0x31, 0x33,
                                0x20, 0x32, 0x30, 0x3a, 0x31, 0x33, 0x3a, 0x32,
                                0x31, 0x20, 0x47, 0x4d, 0x54};
  const mh2c::byte_array_t expected_encoded_data{
      0xd0, 0x7a, 0xbe, 0x94, 0x10, 0x54, 0xd4, 0x44, 0xa8, 0x20, 0x05,
      0x95, 0x04, 0x0b, 0x81, 0x66, 0xe0, 0x82, 0xa6, 0x2d, 0x1b, 0xff};
  const auto encoded_data = mh2c::huffman::encode(data);
  EXPECT_EQ(expected_encoded_data, encoded_data);
}

TEST(huffman_encoder, simple_data_url) {
  // expectead_data: https://www.example.com
  const mh2c::byte_array_t data{0x68, 0x74, 0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f,
                                0x77, 0x77, 0x77, 0x2e, 0x65, 0x78, 0x61, 0x6d,
                                0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d};
  const mh2c::byte_array_t expected_encoded_data{
      0x9d, 0x29, 0xad, 0x17, 0x18, 0x63, 0xc7, 0x8f, 0x0b,
      0x97, 0xc8, 0xe9, 0xae, 0x82, 0xae, 0x43, 0xd3};
  const auto encoded_data = mh2c::huffman::encode(data);
  EXPECT_EQ(expected_encoded_data, encoded_data);
}

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
#include "mh2c/hpack/integer_representation.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"

// Tests for mh2c::encode_interger_value
TEST(encode_integer_value, 7bit_1byte_low_limit) {
  size_t value = 0;
  const auto encoded_value = mh2c::encode_integer_value<7>(value);
  const mh2c::byte_array_t expected_encoded_value = {0};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 7bit_1byte_high_limit) {
  size_t value = 126;
  const auto encoded_value = mh2c::encode_integer_value<7>(value);
  const mh2c::byte_array_t expected_encoded_value = {126};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 7bit_2byte) {
  size_t value = 127;
  const auto encoded_value = mh2c::encode_integer_value<7>(value);
  const mh2c::byte_array_t expected_encoded_value = {127, 0};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 7bit_3byte) {
  size_t value = 255;
  const auto encoded_value = mh2c::encode_integer_value<7>(value);
  const mh2c::byte_array_t expected_encoded_value = {127, 128, 1};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 6bit_1byte_low_limit) {
  size_t value = 0;
  const auto encoded_value = mh2c::encode_integer_value<6>(value);
  const mh2c::byte_array_t expected_encoded_value = {0};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 6bit_1byte_high_limit) {
  size_t value = 62;
  const auto encoded_value = mh2c::encode_integer_value<6>(value);
  const mh2c::byte_array_t expected_encoded_value = {62};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 6bit_2byte) {
  size_t value = 63;
  const auto encoded_value = mh2c::encode_integer_value<6>(value);
  const mh2c::byte_array_t expected_encoded_value = {63, 0};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 6bit_3byte) {
  size_t value = 191;
  const auto encoded_value = mh2c::encode_integer_value<6>(value);
  const mh2c::byte_array_t expected_encoded_value = {63, 128, 1};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 4bit_1byte_low_limit) {
  size_t value = 0;
  const auto encoded_value = mh2c::encode_integer_value<4>(value);
  const mh2c::byte_array_t expected_encoded_value = {0};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 4bit_1byte_high_limit) {
  size_t value = 14;
  const auto encoded_value = mh2c::encode_integer_value<4>(value);
  const mh2c::byte_array_t expected_encoded_value = {14};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 4bit_2byte) {
  size_t value = 15;
  const auto encoded_value = mh2c::encode_integer_value<4>(value);
  const mh2c::byte_array_t expected_encoded_value = {15, 0};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

TEST(encode_integer_value, 4bit_3byte) {
  size_t value = 143;
  const auto encoded_value = mh2c::encode_integer_value<4>(value);
  const mh2c::byte_array_t expected_encoded_value = {15, 128, 1};
  EXPECT_EQ(expected_encoded_value, encoded_value);
}

// Tests for mh2c::decode_interger_value
TEST(decode_integer_value, 7bit_1byte_low_limit) {
  size_t expected_value = 0;
  const auto decoded_value = mh2c::decode_integer_value<7>(
      mh2c::encode_integer_value<7>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 7bit_1byte_high_limit) {
  size_t expected_value = 126;
  const auto decoded_value = mh2c::decode_integer_value<7>(
      mh2c::encode_integer_value<7>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 7bit_2byte) {
  size_t expected_value = 127;
  const auto decoded_value = mh2c::decode_integer_value<7>(
      mh2c::encode_integer_value<7>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 7bit_3byte) {
  size_t expected_value = 255;
  const auto decoded_value = mh2c::decode_integer_value<7>(
      mh2c::encode_integer_value<7>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 6bit_1byte_low_limit) {
  size_t expected_value = 0;
  const auto decoded_value = mh2c::decode_integer_value<6>(
      mh2c::encode_integer_value<6>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 6bit_1byte_high_limit) {
  size_t expected_value = 62;
  const auto decoded_value = mh2c::decode_integer_value<6>(
      mh2c::encode_integer_value<6>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 6bit_2byte) {
  size_t expected_value = 63;
  const auto decoded_value = mh2c::decode_integer_value<6>(
      mh2c::encode_integer_value<6>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 6bit_3byte) {
  size_t expected_value = 191;
  const auto decoded_value = mh2c::decode_integer_value<6>(
      mh2c::encode_integer_value<6>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 4bit_1byte_low_limit) {
  size_t expected_value = 0;
  const auto decoded_value = mh2c::decode_integer_value<4>(
      mh2c::encode_integer_value<4>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 4bit_1byte_high_limit) {
  size_t expected_value = 14;
  const auto decoded_value = mh2c::decode_integer_value<4>(
      mh2c::encode_integer_value<4>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 4bit_2byte) {
  size_t expected_value = 15;
  const auto decoded_value = mh2c::decode_integer_value<4>(
      mh2c::encode_integer_value<4>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

TEST(decode_integer_value, 4bit_3byte) {
  size_t expected_value = 143;
  const auto decoded_value = mh2c::decode_integer_value<4>(
      mh2c::encode_integer_value<4>(expected_value));
  EXPECT_EQ(expected_value, decoded_value);
}

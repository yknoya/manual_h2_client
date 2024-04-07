// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
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

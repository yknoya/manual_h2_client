// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/util/bit_operation.h"

#include <gtest/gtest.h>

#include <cstdint>

TEST(bit_operation_test, extract_low_bit_1) {
  const uint8_t bits = 0b11111110u;
  const uint8_t expected_result = 0b0u;

  const auto result = mh2c::extract_low_bit<1>(bits);
  EXPECT_EQ(expected_result, result);
}

TEST(bit_operation_test, extract_low_bit_7) {
  const uint8_t bits = 0b10101010u;
  const uint8_t expected_result = 0b0101010u;

  const auto result = mh2c::extract_low_bit<7>(bits);
  EXPECT_EQ(expected_result, result);
}

TEST(bit_operation_test, extract_high_bit_1) {
  const uint8_t bits = 0b10000000u;
  const uint8_t expected_result = 0b1u;

  const auto result = mh2c::extract_high_bit<1>(bits);
  EXPECT_EQ(expected_result, result);
}

TEST(bit_operation_test, extract_high_bit_7) {
  const uint8_t bits = 0b10101010u;
  const uint8_t expected_result = 0b1010101u;

  const auto result = mh2c::extract_high_bit<7>(bits);
  EXPECT_EQ(expected_result, result);
}

TEST(bit_operation_test, is_flag_set_same_type_true) {
  const uint8_t flags = 0b10001111;
  const uint8_t flag = 0b10000000;
  const auto result = mh2c::is_flag_set(flags, flag);
  EXPECT_TRUE(result);
}

TEST(bit_operation_test, is_flag_set_same_type_false) {
  const uint8_t flags = 0b00001111;
  const uint8_t flag = 0b10000000;
  const auto result = mh2c::is_flag_set(flags, flag);
  EXPECT_FALSE(result);
}

TEST(bit_operation_test, is_flag_set_different_type_true) {
  const uint16_t flags = 0b1111111101000000;
  const uint8_t flag = 0b01000000;
  const auto result = mh2c::is_flag_set(flags, flag);
  EXPECT_TRUE(result);
}

TEST(bit_operation_test, is_flag_set_different_type_false) {
  const uint16_t flags = 0b1111111100000000;
  const uint8_t flag = 0b01000000;
  const auto result = mh2c::is_flag_set(flags, flag);
  EXPECT_FALSE(result);
}

TEST(bit_operation_test, is_flag_set_enum_type_true) {
  const uint8_t flags = 0b10001111;
  enum class test_flag { TEST = 0b10000000 };
  const test_flag flag{test_flag::TEST};
  const auto result = mh2c::is_flag_set(flags, flag);
  EXPECT_TRUE(result);
}

TEST(bit_operation_test, is_flag_set_enum_type_false) {
  const uint8_t flags = 0b10001111;
  enum class test_flag { TEST = 0b01000000 };
  const test_flag flag{test_flag::TEST};
  const auto result = mh2c::is_flag_set(flags, flag);
  EXPECT_FALSE(result);
}

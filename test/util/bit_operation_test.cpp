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

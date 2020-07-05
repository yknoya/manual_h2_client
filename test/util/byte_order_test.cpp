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
#include "mh2c/util/byte_order.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>

#include "mh2c/common/byte_array.h"

TEST(byte_order_test, cvt_net2host_uint16_t) {
  constexpr uint16_t expected_result = 0x1234;
  const auto result = mh2c::cvt_host2net(mh2c::cvt_net2host(expected_result));
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, cvt_net2host_uint32_t) {
  constexpr uint32_t expected_result = 0x12345678;
  const auto result = mh2c::cvt_host2net(mh2c::cvt_net2host(expected_result));
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, cvt_host2net_uint16_t) {
  constexpr uint16_t expected_result = 0x1234;
  const auto result = mh2c::cvt_net2host(mh2c::cvt_host2net(expected_result));
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, cvt_host2net_uint32_t) {
  constexpr uint32_t expected_result = 0x12345678;
  const auto result = mh2c::cvt_net2host(mh2c::cvt_host2net(expected_result));
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, bytes2integral_uint8_t) {
  constexpr uint8_t expected_result = 0x12;
  const auto bytes = mh2c::byte_array_t{0x12};
  const auto result =
      mh2c::bytes2integral<std::decay_t<decltype(expected_result)>>(
          bytes.begin());
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, bytes2integral_uint16_t) {
  constexpr uint16_t expected_result = 0x1234;
  const auto bytes = mh2c::byte_array_t{0x12, 0x34};
  const auto result =
      mh2c::bytes2integral<std::decay_t<decltype(expected_result)>>(
          bytes.begin());
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, bytes2integral_uint32_t) {
  constexpr uint32_t expected_result = 0x12345678;
  const auto bytes = mh2c::byte_array_t{0x12, 0x34, 0x56, 0x78};
  const auto result =
      mh2c::bytes2integral<std::decay_t<decltype(expected_result)>>(
          bytes.begin());
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, integral2bytes_uint8_t) {
  constexpr uint8_t integer = 0x12;
  const auto expected_result = mh2c::byte_array_t{0x12};
  const auto result =
      mh2c::integral2bytes<std::decay_t<decltype(expected_result)>>(integer);
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, integral2bytes_uint16_t) {
  constexpr uint16_t integer = 0x1234;
  const auto expected_result = mh2c::byte_array_t{0x12, 0x34};
  const auto result =
      mh2c::integral2bytes<std::decay_t<decltype(expected_result)>>(integer);
  EXPECT_EQ(expected_result, result);
}

TEST(byte_order_test, integral2bytes_uint32_t) {
  constexpr uint32_t integer = 0x12345678;
  const auto expected_result = mh2c::byte_array_t{0x12, 0x34, 0x56, 0x78};
  const auto result =
      mh2c::integral2bytes<std::decay_t<decltype(expected_result)>>(integer);
  EXPECT_EQ(expected_result, result);
}

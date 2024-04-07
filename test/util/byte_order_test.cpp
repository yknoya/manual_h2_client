// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
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

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
#include "mh2c/hpack/header_decoder.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_encoder.h"
#include "mh2c/hpack/header_type.h"

// Tests for indexed header field representation
// cf. https://tools.ietf.org/html/rfc7541#section-6.1
TEST(header_decoder, indexed_header) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::INDEXED, {":method", "GET"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

// Tests for literal header field with incremental indexing
// cf. https://tools.ietf.org/html/rfc7541#section-6.2.1
TEST(header_decoder, indexed_name_INCREMENTAL_INDEXING) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
      {":authority", "example.com"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

TEST(header_decoder, new_name_INCREMENTAL_INDEXING) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

// Tests for literal header field without indexing
// cf. https://tools.ietf.org/html/rfc7541#section-6.2.2
TEST(header_decoder, indexed_name_WITHOUT_INDEXING) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING,
      {":authority", "example.com"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

TEST(header_decoder, new_name_WITHOUT_INDEXING) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

// Tests for literal header field never indexed
// cf. https://tools.ietf.org/html/rfc7541#section-6.2.3
TEST(header_decoder, indexed_name_NEVER_INDEXED) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {":authority", "example.com"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

TEST(header_decoder, new_name_NEVER_INDEXED) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

TEST(header_decoder, SIZE_UPDATED) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::SIZE_UPDATE, 8192u};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

// Test for header with Huffman encode
TEST(header_decoder, decode_huffman_encoded_header) {
  const mh2c::header_block_entry expected_header_entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::HUFFMAN;
  const mh2c::dynamic_table request_dynamic_table{};
  const auto encoded_header =
      mh2c::encode_header(expected_header_entry, mode, request_dynamic_table);

  const mh2c::dynamic_table response_dynamic_table{};
  const auto decoded_header =
      mh2c::decode_header(encoded_header, response_dynamic_table);
  EXPECT_EQ(expected_header_entry, decoded_header.first);
  EXPECT_EQ(encoded_header.size(), decoded_header.second);
}

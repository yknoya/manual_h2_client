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
#include "mh2c/hpack/header_encoder.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

// Tests for indexed header field representation
// cf. https://tools.ietf.org/html/rfc7541#section-6.1
TEST(header_encoder, indexed_header) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::INDEXED, {":method", "GET"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x82,  // index
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

// Tests for literal header field with incremental indexing
// cf. https://tools.ietf.org/html/rfc7541#section-6.2.1
TEST(header_encoder, indexed_name_INCREMENTAL_INDEXING) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
      {":authority", "example.com"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x41,                    // index
      0x0b,                    // header value length
      0x65, 0x78, 0x61, 0x6d,  // header value
      0x70, 0x6c, 0x65, 0x2e,  // header value
      0x63, 0x6f, 0x6d,        // header value
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

TEST(header_encoder, new_name_INCREMENTAL_INDEXING) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x40,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

// Tests for literal header field without indexing
// cf. https://tools.ietf.org/html/rfc7541#section-6.2.2
TEST(header_encoder, indexed_name_WITHOUT_INDEXING) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING,
      {":authority", "example.com"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x01,                    // index
      0x0b,                    // header value length
      0x65, 0x78, 0x61, 0x6d,  // header value
      0x70, 0x6c, 0x65, 0x2e,  // header value
      0x63, 0x6f, 0x6d,        // header value
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

TEST(header_encoder, new_name_WITHOUT_INDEXING) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

// Tests for literal header field never indexed
// cf. https://tools.ietf.org/html/rfc7541#section-6.2.3
TEST(header_encoder, indexed_name_NEVER_INDEXED) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {":authority", "example.com"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x11,                    // index
      0x0b,                    // header value length
      0x65, 0x78, 0x61, 0x6d,  // header value
      0x70, 0x6c, 0x65, 0x2e,  // header value
      0x63, 0x6f, 0x6d,        // header value
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

TEST(header_encoder, new_name_NEVER_INDEXED) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x10,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

// Test for max size update
// cf. https://tools.ietf.org/html/rfc7541#section-6.3
TEST(header_encoder, SIZE_UPDATE) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::SIZE_UPDATE, 8192u};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t expected_encoded_header{
      0x3f,  // prefix and max size
      0xe1,  // max size
      0x3f,  // max-size
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

// Test for header with Huffman encode
TEST(header_encoder, encode_with_huffman_encode) {
  const mh2c::header_block_entry header_entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"hoge", "fuga"}};
  const auto mode = mh2c::header_encode_mode::HUFFMAN;
  const mh2c::byte_array_t expected_encoded_header{
      0x10,              // index
      0x83,              // header name length
      0x9c, 0xf3, 0x17,  // header name value (apply huffman encode)
      0x83,              // header value length
      0x96, 0xd9, 0x87,  // header value (apply huffman encode)
  };
  const mh2c::dynamic_table dynamic_table{};

  const auto encoded_header =
      mh2c::encode_header(header_entry, mode, dynamic_table);
  EXPECT_EQ(expected_encoded_header, encoded_header);
}

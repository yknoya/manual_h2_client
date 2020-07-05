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
#include "mh2c/frame/continuation_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

TEST(continuation_frame_test, serialize_indexed_header) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::INDEXED,
                              {"accept-encoding", "gzip, deflate"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x01,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x90,                    // index
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_indexed_header_with_end_headers) {
  const mh2c::fh_flags_t flags{
      static_cast<mh2c::fh_flags_t>(mh2c::cf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              {"accept-encoding", "gzip, deflate"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x01,        // length
      0x09,                    // type
      0x04,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x90,                    // index
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_indexed_header_name) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED,
      {"if-modified-since", "Fri, 28 Aug 2020 10:00:00 GMT"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x20,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x1f, 0x19,              // index
      0x1d,                    // header value length
      0x46, 0x72, 0x69, 0x2c,  // header value
      0x20, 0x32, 0x38, 0x20,  // header value
      0x41, 0x75, 0x67, 0x20,  // header value
      0x32, 0x30, 0x32, 0x30,  // header value
      0x20, 0x31, 0x30, 0x3a,  // header value
      0x30, 0x30, 0x3a, 0x30,  // header value
      0x30, 0x20, 0x47, 0x4d,  // header value
      0x54,
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_indexed_header_name_empty_value) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"age", ""})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x03,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x1f, 0x06,              // index
      0x00,                    // header value length
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_literal_header) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"hoge", "fuga"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x0b,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x10,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_literal_header_same_value) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED, {":status-dummy", "200"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x13,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x10,                    // index
      0x0d,                    // header name length
      0x3a, 0x73, 0x74, 0x61,  // header name value
      0x74, 0x75, 0x73, 0x2d,  // header name value
      0x64, 0x75, 0x6d, 0x6d,  // header name value
      0x79,                    // header name value
      0x03,                    // header value length
      0x32, 0x30, 0x30,        // header value
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_literal_header_incremental_indexing) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING, {"hoge", "fuga"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x0b,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x40,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_literal_header_without_indexing) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, {"hoge", "fuga"})};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x0b,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_size_update) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      {mh2c::header_prefix_pattern::SIZE_UPDATE, 1024u}};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x03,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x3f,                    // prefix and max size
      0xe1,                    // max size
      0x07,                    // max-size
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_literal_header_with_huffman_code) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING, {"hoge", "fuga"})};
  const auto mode = mh2c::header_encode_mode::HUFFMAN;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x09,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x40,                    // index
      0x83,                    // header name length
      0x9c, 0xf3, 0x17,        // header name value (apply huffman encode)
      0x83,                    // header value length
      0x96, 0xd9, 0x87,        // header value (apply huffman encode)
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, multi_headers) {
  const mh2c::fh_flags_t flags{
      static_cast<mh2c::fh_flags_t>(mh2c::cf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED,
      mh2c::headers_t{
          {"accept-encoding", "gzip, deflate"},
          {"if-modified-since", "Fri, 28 Aug 2020 10:00:00 GMT"},
      })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x21,        // length
      0x09,                    // type
      0x04,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x90,                    // index (accept-encoding: gzip, deflate)
      0x1f, 0x19,              // index (if-modified-since)
      0x1d,                    // header value length
      0x46, 0x72, 0x69, 0x2c,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x20, 0x32, 0x38, 0x20,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x41, 0x75, 0x67, 0x20,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x32, 0x30, 0x32, 0x30,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x20, 0x31, 0x30, 0x3a,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x30, 0x30, 0x3a, 0x30,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x30, 0x20, 0x47, 0x4d,  // header value (Fri, 28 Aug 2020 10:00:00 GMT)
      0x54,                    // header value (Fri, 28 Aug 2020 10:00:00 GMT)
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

TEST(continuation_frame_test, serialize_indexed_header_in_dynamic_table) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_t target_header{"hoge", "fuga"};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, target_header)};
  const auto mode = mh2c::header_encode_mode::NONE;
  mh2c::dynamic_table dynamic_table{};
  dynamic_table.push(target_header);
  const mh2c::continuation_frame cf(flags, stream_id, header_block, mode,
                                    dynamic_table);

  const mh2c::byte_array_t expected_serialized_cf{
      0x00, 0x00, 0x01,        // length
      0x09,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0xbe,                    // index
  };

  const auto serialized_cf = cf.serialize();
  EXPECT_EQ(expected_serialized_cf, serialized_cf);
}

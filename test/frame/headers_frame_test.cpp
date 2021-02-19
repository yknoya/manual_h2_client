// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/headers_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

TEST(headers_frame_test, serialize_indexed_header) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::INDEXED, mh2c::header_t{":method", "GET"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x01,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x82,                    // index
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_indexed_header_name) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::header_t{":authority", "example.com"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x0d,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x11,                    // index
      0x0b,                    // header value length
      0x65, 0x78, 0x61, 0x6d,  // header value
      0x70, 0x6c, 0x65, 0x2e,  // header value
      0x63, 0x6f, 0x6d,        // header value
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_indexed_header_name_empty_value) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED, mh2c::header_t{"age", ""}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x03,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x1f, 0x06,              // index
      0x00,                    // header value length
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x0b,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x10,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header_same_value) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::header_t{":status-dummy", "200"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x13,        // length
      0x01,                    // type
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

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header_incremental_indexing) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x0b,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x40,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header_without_indexing) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x0b,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_size_update) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      {{mh2c::header_prefix_pattern::SIZE_UPDATE, 1024u}});
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x03,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x3f,                    // prefix and max size
      0xe1,                    // max size
      0x07,                    // max-size
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header_with_padding) {
  const mh2c::fh_flags_t flags{
      static_cast<mh2c::fh_flags_t>(mh2c::hf_flag::PADDED)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::byte_array_t padding{0xbe, 0xef};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table, padding);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x0e,        // length
      0x01,                    // type
      0x08,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x02,                    // pad length
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
      0xbe, 0xef,              // padding
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header_with_priority) {
  const mh2c::fh_flags_t flags{
      static_cast<mh2c::fh_flags_t>(mh2c::hf_flag::PRIORITY)};
  const mh2c::fh_stream_id_t stream_id{3u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::byte_array_t padding{};
  const mh2c::hf_priority_option priority_option{1u, 1u, 32u};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table, padding, priority_option);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x10,        // length
      0x01,                    // type
      0x20,                    // flags
      0x00, 0x00, 0x00, 0x03,  // reserved and stream id
      0x80, 0x00, 0x00, 0x01,  // stream dependency
      0x20,                    // weight
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_literal_header_with_huffman_code) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::HUFFMAN;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x09,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x40,                    // index
      0x83,                    // header name length
      0x9c, 0xf3, 0x17,        // header name value (apply huffman encode)
      0x83,                    // header value length
      0x96, 0xd9, 0x87,        // header value (apply huffman encode)
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, multi_headers) {
  const mh2c::fh_flags_t flags{
      static_cast<mh2c::fh_flags_t>(mh2c::hf_flag::END_STREAM) |
      static_cast<mh2c::fh_flags_t>(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/httpbin/headers"},
                                  {":scheme", "https"},
                                  {":authority", "nghttp2.org"},
                              }));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x27,        // length
      0x01,                    // type
      0x05,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x82,                    // index (:mehotd: GET)
      0x10,                    // index (:path: /httpbin/headers)
      0x05,                    // header name length (:path)
      0x3a, 0x70, 0x61, 0x74,  // header name (:path)
      0x68,                    // header name (:path)
      0x10,                    // header value length (/httpbin/headers)
      0x2f, 0x68, 0x74, 0x74,  // header value (/httpbin/headers)
      0x70, 0x62, 0x69, 0x6e,  // header value (/httpbin/headers)
      0x2f, 0x68, 0x65, 0x61,  // header value (/httpbin/headers)
      0x64, 0x65, 0x72, 0x73,  // header value (/httpbin/headers)
      0x87,                    // index (:scheme: https)
      0x11,                    // index (:authority: nghttp2.org)
      0x0b,                    // header value length (nghttp2.org)
      0x6e, 0x67, 0x68, 0x74,  // header vallue (nghttp2.org)
      0x74, 0x70, 0x32, 0x2e,  // header vallue (nghttp2.org)
      0x6f, 0x72, 0x67,        // header vallue (nghttp2.org)
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

TEST(headers_frame_test, serialize_indexed_header_in_dynamic_table) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_t target_header{"hoge", "fuga"};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, target_header));
  const auto mode = mh2c::header_encode_mode::NONE;
  mh2c::dynamic_table dynamic_table{};
  dynamic_table.push(target_header);
  const mh2c::headers_frame hf(flags, stream_id, header_block, mode,
                               dynamic_table);

  const mh2c::byte_array_t expected_serialized_hf{
      0x00, 0x00, 0x01,        // length
      0x01,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0xbe,                    // index
  };

  const auto serialized_hf = hf.serialize();
  EXPECT_EQ(expected_serialized_hf, serialized_hf);
}

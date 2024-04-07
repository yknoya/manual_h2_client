// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/push_promise_frame.h"

#include <gtest/gtest.h>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

TEST(push_promise_frame, serialize_indexed_header) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::INDEXED, mh2c::header_t{":status", "200"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x05,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x88,                    // index
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame, serialize_indexed_header_name) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::header_t{"content-length", "28"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x09,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x1f, 0x0d,              // index
      0x02,                    // header value length
      0x32, 0x38,              // header value
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame, serialize_indexed_header_name_empty_value) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED, mh2c::header_t{"age", ""}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x07,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x1f, 0x06,              // index
      0x00,                    // header value length
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame, serialize_literal_header) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x0f,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x10,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame, serialize_literal_header_same_value) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::header_t{":status-dummy", "200"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x17,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x10,                    // index
      0x0d,                    // header name length
      0x3a, 0x73, 0x74, 0x61,  // header name value
      0x74, 0x75, 0x73, 0x2d,  // header name value
      0x64, 0x75, 0x6d, 0x6d,  // header name value
      0x79,                    // header name value
      0x03,                    // header value length
      0x32, 0x30, 0x30,        // header value
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame_test, serialize_literal_header_incremental_indexing) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x0f,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x40,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame_test, serialize_literal_header_without_indexing) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x0f,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame_test, serialize_size_update) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      {{mh2c::header_prefix_pattern::SIZE_UPDATE, 1024u}});
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x07,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x3f,                    // prefix and max size
      0xe1,                    // max size
      0x07,                    // max-size
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame_test, serialize_literal_header_with_padding) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::ppf_flag::PADDED)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {0xbe, 0xef}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x12,        // length
      0x05,                    // type
      0x08,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x02,                    // pad length
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x00,                    // index
      0x04,                    // header name length
      0x68, 0x6f, 0x67, 0x65,  // header name value
      0x04,                    // header value length
      0x66, 0x75, 0x67, 0x61,  // header value
      0xbe, 0xef,              // padding
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame_test, serialize_literal_header_with_huffman_code) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(
      mh2c::make_header_block(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
                              mh2c::header_t{"hoge", "fuga"}));
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const auto mode = mh2c::header_encode_mode::HUFFMAN;
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x0d,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x40,                    // index
      0x83,                    // header name length
      0x9c, 0xf3, 0x17,        // header name value (apply huffman encode)
      0x83,                    // header value length
      0x96, 0xd9, 0x87,        // header value (apply huffman encode)
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame, serialize_multi_headers) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::ppf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED, mh2c::headers_t{
                                                      {":status", "200"},
                                                      {"content-length", "28"},
                                                  }));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table dynamic_table{};
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x0a,        // length
      0x05,                    // type
      0x04,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0x88,                    // index (:status: 200)
      0x1f, 0x0d,              // index (content-length)
      0x02,                    // header value length
      0x32, 0x38,              // header value (28)
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

TEST(push_promise_frame_test, serialize_indexed_header_in_dynamic_table) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_t target_header{"hoge", "fuga"};
  const mh2c::header_block_t header_block(mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, target_header));
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  mh2c::dynamic_table dynamic_table{};
  dynamic_table.push(target_header);
  const mh2c::push_promise_frame ppf(flags, stream_id, payload, mode,
                                     dynamic_table);

  const mh2c::byte_array_t expected_serialized_ppf{
      0x00, 0x00, 0x05,        // length
      0x05,                    // type
      0x00,                    // flags
      0x00, 0x00, 0x00, 0x01,  // reserved and stream id
      0x00, 0x00, 0x00, 0x03,  // reserved and promised stream id
      0xbe,                    // index
  };

  const auto serialized_ppf = ppf.serialize();
  EXPECT_EQ(expected_serialized_ppf, serialized_ppf);
}

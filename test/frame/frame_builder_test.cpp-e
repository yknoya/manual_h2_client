// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/frame/frame_builder.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/continuation_frame.h"
#include "mh2c/frame/data_frame.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/goaway_frame.h"
#include "mh2c/frame/headers_frame.h"
#include "mh2c/frame/ping_frame.h"
#include "mh2c/frame/priority_frame.h"
#include "mh2c/frame/push_promise_frame.h"
#include "mh2c/frame/rst_stream_frame.h"
#include "mh2c/frame/settings_frame.h"
#include "mh2c/frame/window_update_frame.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

namespace {

template <typename Frame>
mh2c::byte_array_t extract_payload(const Frame& frame) {
  const auto serialized_frame = frame.serialize();

  mh2c::byte_array_t raw_payload{};
  std::copy(serialized_frame.begin() + mh2c::FRAME_HEADER_BYTES,
            serialized_frame.end(), std::back_inserter(raw_payload));

  return raw_payload;
}

}  // namespace

// Build CONTINUATION FRAME
TEST(frame_builder_test, continuation_frame_no_flags) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING,
      mh2c::headers_t{
          {"accept-encoding", "gzip, deflate"},
          {"if-modified-since", "Fri, 28 Aug 2020 10:00:00 GMT"},
      })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::continuation_frame expected_cf{flags, stream_id, header_block,
                                             mode, request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_cf);
  mh2c::dynamic_table response_dynamic_table{};
  const auto frame = mh2c::build_frame(expected_cf.get_header(), raw_payload,
                                       response_dynamic_table);

  EXPECT_EQ(expected_cf, *dynamic_cast<mh2c::continuation_frame*>(frame.get()));
}

TEST(frame_builder_test, continuation_frame_with_end_headers) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING,
      mh2c::headers_t{
          {"accept-encoding", "gzip, deflate"},
          {"if-modified-since", "Fri, 28 Aug 2020 10:00:00 GMT"},
      })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::continuation_frame expected_cf{flags, stream_id, header_block,
                                             mode, request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_cf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_cf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_cf, *dynamic_cast<mh2c::continuation_frame*>(frame.get()));
}

TEST(frame_builder_test, continuation_frame_never_indexed) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::NEVER_INDEXED,
      mh2c::headers_t{
          {"accept-encoding", "gzip, deflate"},
          {"if-modified-since", "Fri, 28 Aug 2020 10:00:00 GMT"},
      })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::continuation_frame expected_cf{flags, stream_id, header_block,
                                             mode, request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_cf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_cf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_cf, *dynamic_cast<mh2c::continuation_frame*>(frame.get()));
}

TEST(frame_builder_test, continuation_frame_incremental_indexing) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{mh2c::make_header_block(
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
      mh2c::headers_t{
          {"accept-encoding", "gzip, deflate"},
          {"if-modified-since", "Fri, 28 Aug 2020 10:00:00 GMT"},
      })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::continuation_frame expected_cf{flags, stream_id, header_block,
                                             mode, request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_cf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_cf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_cf, *dynamic_cast<mh2c::continuation_frame*>(frame.get()));
}

// Build DATA FRAME
TEST(frame_builder_test, data_frame_no_flags) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const std::string payload{"test"};
  const mh2c::data_frame expected_df{flags, stream_id, payload};

  mh2c::byte_array_t raw_payload = extract_payload(expected_df);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_df.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_df, *dynamic_cast<mh2c::data_frame*>(frame.get()));
}

TEST(frame_builder_test, data_frame_with_end_stream) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::df_flag::END_STREAM)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const std::string payload{"test"};
  const mh2c::data_frame expected_df{flags, stream_id, payload};

  mh2c::byte_array_t raw_payload = extract_payload(expected_df);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_df.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_df, *dynamic_cast<mh2c::data_frame*>(frame.get()));
}

TEST(frame_builder_test, data_frame_with_padded) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::df_flag::PADDED)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::byte_array_t payload{0x06, 't', 'e', 's', 't', 'p',
                                   'a',  'd', 'd', 'e', 'd'};
  const mh2c::data_frame expected_df{flags, stream_id, payload};

  mh2c::byte_array_t raw_payload = extract_payload(expected_df);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_df.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_df, *dynamic_cast<mh2c::data_frame*>(frame.get()));
}

// Build GOAWAY FRAME
TEST(frame_builder_test, goaway_frame_no_additional_debug_data) {
  const mh2c::goaway_payload gp{0, 0, mh2c::error_codes::NO_ERROR, {}};
  const mh2c::goaway_frame expected_gf{gp};

  mh2c::byte_array_t raw_payload = extract_payload(expected_gf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_gf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_gf, *dynamic_cast<mh2c::goaway_frame*>(frame.get()));
}

TEST(frame_builder_test, goaway_frame_with_additional_debug_data) {
  const mh2c::goaway_payload gp{
      0, 0, mh2c::error_codes::PROTOCOL_ERROR, {'t', 'e', 's', 't'}};
  const mh2c::goaway_frame expected_gf{gp};

  mh2c::byte_array_t raw_payload = extract_payload(expected_gf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_gf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_gf, *dynamic_cast<mh2c::goaway_frame*>(frame.get()));
}

// Build HEADERS FRAME
TEST(frame_builder_test, headers_frame_no_flags) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{flags, stream_id, header_block, mode,
                                        request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_with_end_stream) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_STREAM)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{flags, stream_id, header_block, mode,
                                        request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_with_end_headers) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{flags, stream_id, header_block, mode,
                                        request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_never_indexed) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{flags, stream_id, header_block, mode,
                                        request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_incremental_indexing) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{flags, stream_id, header_block, mode,
                                        request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_with_padding) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::PADDED)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const mh2c::byte_array_t padding = {'B', 'E', 'E', 'F'};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{flags, stream_id, header_block, mode,
                                        request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_with_priority) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::PRIORITY)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t padding{};
  const mh2c::hf_priority_option priority_option{1u, 12345u, 127u};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{
      flags,   stream_id,      header_block, mode, request_dynamic_table,
      padding, priority_option};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

TEST(frame_builder_test, headers_frame_with_padding_and_priority) {
  const mh2c::fh_flags_t flags{mh2c::make_frame_header_flags(
      mh2c::hf_flag::PADDED, mh2c::hf_flag::PRIORITY)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":method", "GET"},
                                  {":path", "/"},
                                  {":scheme", "https"},
                                  {":authority", "example.com"},
                                  {"accept", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t padding{'B', 'E', 'E', 'F'};
  const mh2c::hf_priority_option priority_option{1u, 12345u, 128u};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::headers_frame expected_hf{
      flags,   stream_id,      header_block, mode, request_dynamic_table,
      padding, priority_option};

  mh2c::byte_array_t raw_payload = extract_payload(expected_hf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_hf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_hf, *dynamic_cast<mh2c::headers_frame*>(frame.get()));
}

// Build PING FRAME
TEST(frame_builder_test, ping_frame) {
  const mh2c::byte_array_t opaque_data{'p', 'i', 'n', 'g'};
  const mh2c::ping_frame expected_pf{0u, opaque_data};
  const auto raw_payload = extract_payload(expected_pf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_pf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_pf, *dynamic_cast<mh2c::ping_frame*>(frame.get()));
}

TEST(frame_builder_test, ping_frame_with_ack) {
  const mh2c::byte_array_t opaque_data{'p', 'i', 'n', 'g'};
  const mh2c::ping_frame expected_pf{
      mh2c::make_frame_header_flags(mh2c::pf_flag::ACK), opaque_data};
  const auto raw_payload = extract_payload(expected_pf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_pf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_pf, *dynamic_cast<mh2c::ping_frame*>(frame.get()));
}

// Build PRIORITY FRAME
TEST(frame_builder_test, priority_frame) {
  const mh2c::priority_payload payload{0u, 1u, 0u};
  const mh2c::priority_frame expected_pf(3u, payload);
  const auto raw_payload = extract_payload(expected_pf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_pf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_pf, *dynamic_cast<mh2c::priority_frame*>(frame.get()));
}

TEST(frame_builder_test, priority_frame_with_exclusive) {
  const mh2c::priority_payload payload{1u, 1u, 0u};
  const mh2c::priority_frame expected_pf(3u, payload);
  const auto raw_payload = extract_payload(expected_pf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_pf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_pf, *dynamic_cast<mh2c::priority_frame*>(frame.get()));
}

TEST(frame_builder_test, priority_frame_with_weight) {
  const mh2c::priority_payload payload{1u, 1u, 255u};
  const mh2c::priority_frame expected_pf(3u, payload);
  const auto raw_payload = extract_payload(expected_pf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_pf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_pf, *dynamic_cast<mh2c::priority_frame*>(frame.get()));
}

// Build PUSH_PROMISE_FRAME
TEST(frame_builder_test, push_promise_frame_no_flags) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":status", "200"},
                                  {":content-length", "128"},
                                  {":content-type", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::push_promise_frame expected_ppf{flags, stream_id, payload, mode,
                                              request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_ppf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_ppf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_ppf,
            *dynamic_cast<mh2c::push_promise_frame*>(frame.get()));
}

TEST(frame_builder_test, push_promise_frame_with_end_headers) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::ppf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":status", "200"},
                                  {":content-length", "128"},
                                  {":content-type", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::push_promise_frame expected_ppf{flags, stream_id, payload, mode,
                                              request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_ppf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_ppf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_ppf,
            *dynamic_cast<mh2c::push_promise_frame*>(frame.get()));
}

TEST(frame_builder_test, push_promise_frame_never_indexed) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::NEVER_INDEXED,
                              mh2c::headers_t{
                                  {":status", "200"},
                                  {":content-length", "128"},
                                  {":content-type", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::push_promise_frame expected_ppf{flags, stream_id, payload, mode,
                                              request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_ppf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_ppf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_ppf,
            *dynamic_cast<mh2c::push_promise_frame*>(frame.get()));
}

TEST(frame_builder_test, push_promise_frame_incremental_indexing) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
                              mh2c::headers_t{
                                  {":status", "200"},
                                  {":content-length", "128"},
                                  {":content-type", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::push_promise_payload payload{0u, 3u, header_block, {}};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::push_promise_frame expected_ppf{flags, stream_id, payload, mode,
                                              request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_ppf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_ppf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_ppf,
            *dynamic_cast<mh2c::push_promise_frame*>(frame.get()));
}

TEST(frame_builder_test, push_promise_frame_with_padding) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::hf_flag::PADDED)};
  const mh2c::fh_stream_id_t stream_id{1u};
  const mh2c::header_block_t header_block{
      mh2c::make_header_block(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                              mh2c::headers_t{
                                  {":status", "200"},
                                  {":content-length", "128"},
                                  {":content-type", "application/json"},
                              })};
  const auto mode = mh2c::header_encode_mode::NONE;
  const mh2c::byte_array_t padding = {'B', 'E', 'E', 'F'};
  const mh2c::push_promise_payload payload{0u, 3u, header_block, padding};
  const mh2c::dynamic_table request_dynamic_table{};
  const mh2c::push_promise_frame expected_ppf{flags, stream_id, payload, mode,
                                              request_dynamic_table};

  mh2c::byte_array_t raw_payload = extract_payload(expected_ppf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_ppf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_ppf,
            *dynamic_cast<mh2c::push_promise_frame*>(frame.get()));
}

// Build RST_STREAM FRAME
TEST(frame_builder_test, rst_stream_frame) {
  const mh2c::fh_stream_id_t stream_id{1};
  const mh2c::rst_stream_frame expected_rsf{stream_id,
                                            mh2c::error_codes::NO_ERROR};
  const auto raw_payload = extract_payload(expected_rsf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_rsf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_rsf, *dynamic_cast<mh2c::rst_stream_frame*>(frame.get()));
}

TEST(frame_builder_test, rst_stream_frame_unknown_error_code) {
  const mh2c::fh_stream_id_t stream_id{1};
  const mh2c::rst_stream_frame expected_rsf{stream_id,
                                            mh2c::error_codes::UNKNOWN_ERROR};
  const auto raw_payload = extract_payload(expected_rsf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_rsf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_rsf, *dynamic_cast<mh2c::rst_stream_frame*>(frame.get()));
}

TEST(frame_builder_test, rst_stream_frame_invalid_length) {
  const mh2c::fh_stream_id_t stream_id{1};
  const mh2c::rst_stream_frame expected_rsf{stream_id,
                                            mh2c::error_codes::UNKNOWN_ERROR};
  auto raw_payload = extract_payload(expected_rsf);
  raw_payload.pop_back();
  mh2c::dynamic_table dynamic_table{};

  EXPECT_ANY_THROW(
      mh2c::build_frame(expected_rsf.get_header(), raw_payload, dynamic_table));
}

// Build SETTINGS FRAME
TEST(frame_builder_test, settings_frame_full_settings) {
  const mh2c::fh_flags_t flags{0u};
  const mh2c::fh_stream_id_t stream_id{0u};
  const mh2c::sf_payload_t sf_payload{mh2c::make_sf_payload(
      {{mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE, 512u},
       {mh2c::sf_parameter::SETTINGS_ENABLE_PUSH, 0u},
       {mh2c::sf_parameter::SETTINGS_MAX_CONCURRENT_STREAM, 16u},
       {mh2c::sf_parameter::SETTINGS_INITIAL_WINDOW_SIZE, 65535u},
       {mh2c::sf_parameter::SETTINGS_MAX_FRAME_SIZE, 65535u},
       {mh2c::sf_parameter::SETTINGS_MAX_HEADER_LIST_SIZE, 256u}})};
  const mh2c::settings_frame expected_sf{flags, stream_id, sf_payload};

  mh2c::byte_array_t raw_payload = extract_payload(expected_sf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_sf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_sf, *dynamic_cast<mh2c::settings_frame*>(frame.get()));
}

TEST(frame_builder_test, settings_frame_ack) {
  const mh2c::fh_flags_t flags{
      mh2c::make_frame_header_flags(mh2c::sf_flag::ACK)};
  const mh2c::fh_stream_id_t stream_id{0u};
  const mh2c::sf_payload_t sf_payload{};
  const mh2c::settings_frame expected_sf{flags, stream_id, sf_payload};

  mh2c::byte_array_t raw_payload = extract_payload(expected_sf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_sf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_sf, *dynamic_cast<mh2c::settings_frame*>(frame.get()));
}

// Build WINDOW_UPDATE FRAME
TEST(frame_builder_test, window_update_frame) {
  const mh2c::fh_stream_id_t stream_id{0u};
  const mh2c::window_size_t window_size{0x7fffffff};
  const mh2c::window_update_frame expected_wuf{stream_id, window_size};

  mh2c::byte_array_t raw_payload = extract_payload(expected_wuf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_wuf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_wuf,
            *dynamic_cast<mh2c::window_update_frame*>(frame.get()));
}

TEST(frame_builder_test, window_update_with_reserved) {
  const mh2c::fh_stream_id_t stream_id{0u};
  const mh2c::reserved_t reserved{1u};
  const mh2c::window_size_t window_size{0x01234567};
  const mh2c::window_update_frame expected_wuf{stream_id, reserved,
                                               window_size};

  mh2c::byte_array_t raw_payload = extract_payload(expected_wuf);
  mh2c::dynamic_table dynamic_table{};
  const auto frame =
      mh2c::build_frame(expected_wuf.get_header(), raw_payload, dynamic_table);

  EXPECT_EQ(expected_wuf,
            *dynamic_cast<mh2c::window_update_frame*>(frame.get()));
}

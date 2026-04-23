// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/http2_client_internal.h"

#include <gtest/gtest.h>

#include <memory>

#include "mh2c/frame/continuation_frame.h"
#include "mh2c/frame/headers_frame.h"
#include "mh2c/frame/push_promise_frame.h"
#include "mh2c/frame/settings_frame.h"
#include "mh2c/hpack/dynamic_table.h"
#include "mh2c/hpack/header_type.h"

TEST(http2_client_internal_test,
     update_dynamic_table_from_header_block_updates_size_and_entries) {
  const mh2c::header_t incremental_header{"x-debug", "enabled"};
  const mh2c::header_t without_indexing_header{"x-skip", "value"};
  const mh2c::header_block_t header_block{
      mh2c::header_block_entry{
          mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
          incremental_header},
      mh2c::header_block_entry{mh2c::header_prefix_pattern::WITHOUT_INDEXING,
                               without_indexing_header},
      mh2c::header_block_entry{mh2c::header_prefix_pattern::SIZE_UPDATE, 64u},
  };
  mh2c::dynamic_table table{};

  mh2c::detail::update_dynamic_table(header_block, &table);

  ASSERT_EQ(1u, table.get_entries().size());
  EXPECT_EQ(incremental_header, table.at(0));
  EXPECT_EQ(64u, table.get_max_table_size());
}

TEST(http2_client_internal_test,
     update_dynamic_table_from_settings_frame_updates_table_size) {
  mh2c::dynamic_table table{};
  mh2c::h2_frame_ptr frame = std::make_unique<mh2c::settings_frame>(
      0u, 0u,
      mh2c::make_sf_payload(
          {{mh2c::sf_parameter::SETTINGS_HEADER_TABLE_SIZE, 128u}}));

  mh2c::detail::update_dynamic_table(frame, &table);

  EXPECT_EQ(128u, table.get_max_table_size());
}

TEST(http2_client_internal_test,
     update_dynamic_table_from_headers_frame_pushes_incremental_entries) {
  const mh2c::header_block_t header_block{
      mh2c::header_block_entry{
          mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
          {"x-response-id", "123"}},
  };
  mh2c::h2_frame_ptr frame = std::make_unique<mh2c::headers_frame>(
      mh2c::make_frame_header_flags(mh2c::hf_flag::END_HEADERS), 1u,
      header_block, mh2c::header_encode_mode::NONE, mh2c::dynamic_table{});
  mh2c::dynamic_table table{};

  mh2c::detail::update_dynamic_table(frame, &table);

  ASSERT_EQ(1u, table.get_entries().size());
  EXPECT_EQ(mh2c::header_t("x-response-id", "123"), table.at(0));
}

TEST(http2_client_internal_test,
     update_dynamic_table_from_continuation_frame_pushes_incremental_entries) {
  const mh2c::header_block_t header_block{
      mh2c::header_block_entry{
          mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
          {"x-cont", "part-2"}},
  };
  mh2c::h2_frame_ptr frame = std::make_unique<mh2c::continuation_frame>(
      mh2c::make_frame_header_flags(mh2c::cf_flag::END_HEADERS), 1u,
      header_block, mh2c::header_encode_mode::NONE, mh2c::dynamic_table{});
  mh2c::dynamic_table table{};

  mh2c::detail::update_dynamic_table(frame, &table);

  ASSERT_EQ(1u, table.get_entries().size());
  EXPECT_EQ(mh2c::header_t("x-cont", "part-2"), table.at(0));
}

TEST(http2_client_internal_test,
     update_dynamic_table_from_push_promise_frame_pushes_incremental_entries) {
  const mh2c::push_promise_payload payload{
      0u,
      3u,
      {mh2c::header_block_entry{
          mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
          {"x-promised", "yes"}}},
      {},
  };
  mh2c::h2_frame_ptr frame = std::make_unique<mh2c::push_promise_frame>(
      mh2c::make_frame_header_flags(mh2c::ppf_flag::END_HEADERS), 1u, payload,
      mh2c::header_encode_mode::NONE, mh2c::dynamic_table{});
  mh2c::dynamic_table table{};

  mh2c::detail::update_dynamic_table(frame, &table);

  ASSERT_EQ(1u, table.get_entries().size());
  EXPECT_EQ(mh2c::header_t("x-promised", "yes"), table.at(0));
}

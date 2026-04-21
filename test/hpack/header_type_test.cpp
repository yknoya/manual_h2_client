// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/header_type.h"

#include <gtest/gtest.h>

#include <stdexcept>

TEST(header_type_test, default_constructor_uses_never_indexed_prefix) {
  const mh2c::header_block_entry entry{{"x-test-header", "x-test-value"}};

  EXPECT_EQ(mh2c::header_prefix_pattern::NEVER_INDEXED, entry.get_prefix());
  EXPECT_EQ((mh2c::header_t{"x-test-header", "x-test-value"}),
            entry.get_header());
}

TEST(header_type_test, make_header_block_corrects_exact_static_table_match) {
  const auto header_block = mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING,
      mh2c::header_t{":method", "GET"});

  ASSERT_EQ(1u, header_block.size());
  EXPECT_EQ(mh2c::header_prefix_pattern::INDEXED,
            header_block.front().get_prefix());
  EXPECT_EQ((mh2c::header_t{":method", "GET"}),
            header_block.front().get_header());
}

TEST(header_type_test, make_header_block_keeps_prefix_for_empty_static_value) {
  const auto header_block = mh2c::make_header_block(
      mh2c::header_prefix_pattern::WITHOUT_INDEXING,
      mh2c::header_t{"age", ""});

  ASSERT_EQ(1u, header_block.size());
  EXPECT_EQ(mh2c::header_prefix_pattern::WITHOUT_INDEXING,
            header_block.front().get_prefix());
  EXPECT_EQ((mh2c::header_t{"age", ""}), header_block.front().get_header());
}

TEST(header_type_test, make_header_block_keeps_prefix_for_non_static_header) {
  const auto header_block = mh2c::make_header_block(
      mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
      mh2c::header_t{"x-test-header", "x-test-value"});

  ASSERT_EQ(1u, header_block.size());
  EXPECT_EQ(mh2c::header_prefix_pattern::INCREMENTAL_INDEXING,
            header_block.front().get_prefix());
  EXPECT_EQ((mh2c::header_t{"x-test-header", "x-test-value"}),
            header_block.front().get_header());
}

TEST(header_type_test, get_header_throws_when_entry_holds_size) {
  const mh2c::header_block_entry entry{mh2c::header_prefix_pattern::SIZE_UPDATE,
                                       1024u};

  EXPECT_THROW(entry.get_header(), std::runtime_error);
}

TEST(header_type_test, get_max_size_throws_when_entry_holds_header) {
  const mh2c::header_block_entry entry{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"x-test-header", "value"}};

  EXPECT_THROW(entry.get_max_size(), std::runtime_error);
}

TEST(header_type_test, set_header_updates_variant_contents) {
  mh2c::header_block_entry entry{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, {"name", "before"}};

  entry.set_header({"name", "after"});

  EXPECT_EQ((mh2c::header_t{"name", "after"}), entry.get_header());
}

TEST(header_type_test, equality_depends_on_prefix_and_entry) {
  const mh2c::header_block_entry lhs{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, {"name", "value"}};
  const mh2c::header_block_entry rhs{
      mh2c::header_prefix_pattern::WITHOUT_INDEXING, {"name", "value"}};
  const mh2c::header_block_entry different_prefix{
      mh2c::header_prefix_pattern::NEVER_INDEXED, {"name", "value"}};

  EXPECT_EQ(lhs, rhs);
  EXPECT_NE(lhs, different_prefix);
}

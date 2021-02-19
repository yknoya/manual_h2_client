// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/static_table_definition.h"

#include <gtest/gtest.h>

#include <algorithm>

#include "mh2c/hpack/header_type.h"

TEST(reverse_static_table_entries, reverse_check) {
  mh2c::header_table_t header_table{};
  std::for_each(mh2c::reverse_static_table_entries.begin(),
                mh2c::reverse_static_table_entries.end(),
                [&header_table](const auto pair) {
                  header_table[pair.second] = pair.first;
                });

  EXPECT_EQ(mh2c::static_table_entries, header_table);
}

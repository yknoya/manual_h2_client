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
#include "mh2c/hpack/dynamic_table.h"

#include <gtest/gtest.h>

#include <string>

#include "mh2c/hpack/header_type.h"

namespace {

mh2c::dynamic_table::size_type calculate_header_size(
    const mh2c::header_t& header) {
  return header.first.length() + header.second.length() +
         mh2c::dynamic_table::ENTRY_OVERHEAD_SIZE;
}

}  // namespace

TEST(dynamic_table, push_single_header) {
  const mh2c::header_t authority_header{":authority", "example.com"};
  const auto expected_table_size{calculate_header_size(authority_header)};
  const mh2c::dynamic_table::container_type expected_entries{authority_header};

  mh2c::dynamic_table table{};
  table.push(authority_header);

  EXPECT_EQ(authority_header, table.at(0));
  EXPECT_EQ(expected_entries, table.get_entries());
  EXPECT_EQ(expected_table_size, table.get_table_size());
  EXPECT_EQ(mh2c::dynamic_table::DEFAULT_TABLE_SIZE, table.get_max_table_size());
}

TEST(dynamic_table, push_multiple_header) {
  const mh2c::header_t authority_header{":authority", "example.com"};
  const mh2c::header_t content_type_header{"content-type", "application/json"};
  const mh2c::dynamic_table::container_type expected_entries{content_type_header,
                                                             authority_header};
  const auto expected_table_size{calculate_header_size(authority_header) +
                                 calculate_header_size(content_type_header)};

  mh2c::dynamic_table table{};
  table.push(authority_header);
  table.push(content_type_header);

  EXPECT_EQ(content_type_header, table.at(0));
  EXPECT_EQ(authority_header, table.at(1));
  EXPECT_EQ(expected_entries, table.get_entries());
  EXPECT_EQ(expected_table_size, table.get_table_size());
  EXPECT_EQ(mh2c::dynamic_table::DEFAULT_TABLE_SIZE, table.get_max_table_size());
}

TEST(dynamic_table, resize_table) {
  const mh2c::header_t authority_header{":authority", "example.com"};
  const mh2c::header_t content_type_header{"content-type", "application/json"};
  const auto expected_table_size{calculate_header_size(content_type_header)};

  mh2c::dynamic_table table{expected_table_size};
  table.push(authority_header);
  table.push(content_type_header);

  EXPECT_EQ(content_type_header, table.at(0));
  EXPECT_EQ(expected_table_size, table.get_table_size());
  EXPECT_EQ(expected_table_size, table.get_max_table_size());
}

TEST(dynamic_table, update_table_size) {
  const mh2c::header_t authority_header{":authority", "example.com"};
  const mh2c::header_t content_type_header{"content-type", "application/json"};
  const mh2c::dynamic_table::container_type entries{content_type_header,
                                                    authority_header};
  const auto table_size{calculate_header_size(authority_header) +
                        calculate_header_size(content_type_header)};

  mh2c::dynamic_table table{};
  table.push(authority_header);
  table.push(content_type_header);

  EXPECT_EQ(entries, table.get_entries());
  EXPECT_EQ(table_size, table.get_table_size());
  EXPECT_EQ(mh2c::dynamic_table::DEFAULT_TABLE_SIZE, table.get_max_table_size());

  const mh2c::dynamic_table::container_type updated_entries{entries[0]};
  const auto updated_table_size{calculate_header_size(content_type_header)};
  table.update_table_size(updated_table_size);

  EXPECT_EQ(updated_entries, table.get_entries());
  EXPECT_EQ(updated_table_size, table.get_table_size());
  EXPECT_EQ(updated_table_size, table.get_max_table_size());
}

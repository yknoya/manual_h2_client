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
#include "mh2c/hpack/header_type.h"

#include <algorithm>
#include <stdexcept>

#include "mh2c/hpack/static_table_definition.h"

namespace mh2c {

namespace {

header_block_entry correct_prefix(const header_block_entry& header_entry) {
  const auto header = header_entry.get_header();
  const auto iter = reverse_static_table_entries.find(header);
  if (iter != reverse_static_table_entries.end() && header.second.length() > 0) {
    return header_block_entry{header_prefix_pattern::INDEXED, header};
  }

  return header_entry;
}

}  // namespace

header_block_entry::header_block_entry(const header_t& header)
    : header_block_entry{header_prefix_pattern::NEVER_INDEXED, header} {}

header_block_entry::header_block_entry(const header_prefix_pattern prefix,
                                       const header_t& header)
    : m_prefix{prefix}, m_entry{header} {}

header_block_entry::header_block_entry(const header_prefix_pattern prefix,
                                       const size_t max_size)
    : m_prefix{prefix}, m_entry{max_size} {}

header_prefix_pattern header_block_entry::get_prefix() const { return m_prefix; }

header_t header_block_entry::get_header() const {
  if (std::holds_alternative<header_t>(m_entry)) {
    return std::get<header_t>(m_entry);
  } else {
    throw std::runtime_error("m_entry doesn't hold header_t");
  }
}
size_t header_block_entry::get_max_size() const {
  if (std::holds_alternative<size_t>(m_entry)) {
    return std::get<size_t>(m_entry);
  } else {
    throw std::runtime_error("m_entry doesn't hold size_t");
  }
}

std::variant<header_t, size_t> header_block_entry::get_entry() const {
  return m_entry;
}

void header_block_entry::set_header(const header_t& header) { m_entry = header; }

bool operator==(const header_block_entry& lhs, const header_block_entry& rhs) {
  return lhs.get_prefix() == rhs.get_prefix() &&
         lhs.get_entry() == rhs.get_entry();
}

bool operator!=(const header_block_entry& lhs, const header_block_entry& rhs) {
  return !(lhs == rhs);
}

header_block_t make_header_block(const header_prefix_pattern prefix,
                                 const header_t& header) {
  return make_header_block(prefix, headers_t{header});
}

header_block_t make_header_block(const header_prefix_pattern prefix,
                                 const headers_t& headers) {
  header_block_t header_block{};

  std::transform(headers.begin(), headers.end(), std::back_inserter(header_block),
                 [prefix](const auto& header) {
                   return correct_prefix({prefix, header});
                 });

  return header_block;
}

}  // namespace mh2c

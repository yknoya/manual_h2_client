// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/dynamic_table.h"

#include <algorithm>
#include <ostream>
#include <string>

namespace mh2c {

namespace {

dynamic_table::size_type resize_table(
    dynamic_table::container_type* entries,
    const dynamic_table::size_type table_size,
    const dynamic_table::size_type max_table_size) {
  auto current_table_size = table_size;
  while (current_table_size > max_table_size) {
    const auto& oldest_header = entries->back();
    const auto oldest_header_size = oldest_header.first.length() +
                                    oldest_header.second.length() +
                                    dynamic_table::ENTRY_OVERHEAD_SIZE;
    entries->pop_back();
    current_table_size -= oldest_header_size;
  }
  return current_table_size;
}

}  // namespace

dynamic_table::dynamic_table() : dynamic_table{DEFAULT_TABLE_SIZE} {}

dynamic_table::dynamic_table(const size_type initial_size)
    : m_entries{}, m_table_size{0}, m_max_table_size{initial_size} {}

void dynamic_table::push(const_reference header) {
  const auto header_size =
      header.first.length() + header.second.length() + ENTRY_OVERHEAD_SIZE;
  m_table_size =
      resize_table(&m_entries, m_table_size + header_size, m_max_table_size);
  m_entries.insert(m_entries.begin(), header);

  return;
}

dynamic_table::const_reference dynamic_table::at(const size_t position) const {
  return m_entries.at(position);
}
void dynamic_table::update_table_size(const size_type new_size) {
  m_table_size = resize_table(&m_entries, m_table_size, new_size);
  m_max_table_size = new_size;
}

const dynamic_table::container_type& dynamic_table::get_entries() const {
  return m_entries;
}

const dynamic_table::size_type dynamic_table::get_table_size() const {
  return m_table_size;
}
const dynamic_table::size_type dynamic_table::get_max_table_size() const {
  return m_max_table_size;
}

dynamic_table::iterator dynamic_table::begin() { return m_entries.begin(); }

dynamic_table::const_iterator dynamic_table::begin() const {
  return m_entries.begin();
}

dynamic_table::iterator dynamic_table::end() { return m_entries.end(); }

dynamic_table::const_iterator dynamic_table::end() const {
  return m_entries.end();
}

std::ostream& operator<<(std::ostream& out_stream, const dynamic_table& table) {
  out_stream << "=== DYNAMIC_TABLE ===\n";
  auto table_index = dynamic_table::FIRST_INDEX;
  const auto& entries = table.get_entries();
  std::for_each(entries.begin(), entries.end(),
                [&out_stream, &table_index](const auto& entry) {
                  out_stream << "[" << std::to_string(table_index++) << "] -> "
                             << entry.first << ": " << entry.second << '\n';
                });
  return out_stream;
}

}  // namespace mh2c

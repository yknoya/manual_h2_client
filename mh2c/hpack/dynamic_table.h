// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_HPACK_DYNAMIC_TABLE_H_
#define MH2C_HPACK_DYNAMIC_TABLE_H_

#include <ostream>
#include <vector>

#include "mh2c/hpack/header_type.h"

namespace mh2c {

class dynamic_table {
 public:
  using container_type = std::vector<header_t>;
  using value_type = container_type::value_type;
  using size_type = container_type::size_type;
  using reference = container_type::reference;
  using const_reference = container_type::const_reference;
  using iterator = container_type::iterator;
  using const_iterator = container_type::const_iterator;

  // cf. https://tools.ietf.org/html/rfc7540#section-6.5.2
  static constexpr dynamic_table::size_type DEFAULT_TABLE_SIZE = 4096;
  // cf. https://tools.ietf.org/html/rfc7541#section-4.1
  static constexpr dynamic_table::size_type ENTRY_OVERHEAD_SIZE = 32;
  // cf. https://tools.ietf.org/html/rfc7541#section-2.3.3
  static constexpr dynamic_table::size_type FIRST_INDEX = 62;

  dynamic_table();
  explicit dynamic_table(const size_type initial_size);

  void push(const_reference header);
  const_reference at(const size_t position) const;
  void update_table_size(const size_type new_size);

  const container_type& get_entries() const;
  size_type get_table_size() const;
  size_type get_max_table_size() const;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

 private:
  container_type m_entries;
  size_type m_table_size;
  size_type m_max_table_size;
};

std::ostream& operator<<(std::ostream& out_stream, const dynamic_table& table);

}  // namespace mh2c

#endif  // MH2C_HPACK_DYNAMIC_TABLE_H_

// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_I_FRAME_H_
#define MH2C_FRAME_I_FRAME_H_

#include <ostream>

#include "mh2c/common/byte_array.h"

namespace mh2c {

// Interface of HTTP/2 frame
template <typename Header>
class i_frame {
 public:
  i_frame() = default;
  virtual ~i_frame() = default;

  virtual Header get_header() const = 0;

  virtual byte_array_t serialize() const = 0;
  virtual void dump(std::ostream& out_stream) const = 0;
};

}  // namespace mh2c

#endif  // MH2C_FRAME_I_FRAME_H_

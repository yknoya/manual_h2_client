// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_WINDOW_UPDATE_FRAME_H_
#define MH2C_FRAME_WINDOW_UPDATE_FRAME_H_

#include <cstdint>
#include <ostream>

#include "mh2c/common/byte_array.h"
#include "mh2c/frame/common_type.h"
#include "mh2c/frame/frame_header.h"
#include "mh2c/frame/i_frame.h"

namespace mh2c {

using window_size_t = uint32_t;

class window_update_frame : public i_frame<frame_header> {
 public:
  window_update_frame(const fh_stream_id_t stream_id,
                      const window_size_t window_size_increment);
  window_update_frame(const fh_stream_id_t stream_id, const reserved_t reserved,
                      const window_size_t window_size_increment);
  window_update_frame(const frame_header& fh, const byte_array_t& raw_payload);

  frame_header get_header() const override;
  window_size_t get_payload() const;

  byte_array_t serialize() const override;
  void dump(std::ostream& out_stream) const override;

 private:
  frame_header m_header;
  reserved_t m_reserved;
  window_size_t m_window_size_increment;

  friend bool operator==(const window_update_frame& lhs,
                         const window_update_frame& rhs);
};

std::ostream& operator<<(std::ostream& out_stream,
                         const window_update_frame& wuf);
bool operator==(const window_update_frame& lhs, const window_update_frame& rhs);
bool operator!=(const window_update_frame& lhs, const window_update_frame& rhs);

}  // namespace mh2c

#endif  // MH2C_FRAME_WINDOW_UPDATE_FRAME_H_

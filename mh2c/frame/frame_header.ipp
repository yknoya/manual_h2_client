// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_FRAME_HEADER_IPP_
#define MH2C_FRAME_FRAME_HEADER_IPP_

namespace mh2c {

template <typename T>
inline fh_length_t cast_to_fh_length(T length) {
  return static_cast<fh_length_t>(length);
}

template <typename... Args>
inline fh_flags_t make_frame_header_flags(Args... args) {
  return (... | static_cast<fh_flags_t>(args));
}

}  // namespace mh2c

#endif  // MH2C_FRAME_FRAME_HEADER_IPP_

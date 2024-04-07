// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_ERROR_CODES_IPP_
#define MH2C_FRAME_ERROR_CODES_IPP_

namespace mh2c {

template <typename T>
inline error_codes cast_to_error_codes(T error_code) {
  return static_cast<error_codes>(error_code);
}

}  // namespace mh2c

#endif  // MH2C_FRAME_ERROR_CODES_IPP_

// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_SETTINGS_FRAME_IPP_
#define MH2C_FRAME_SETTINGS_FRAME_IPP_

namespace mh2c {

template <typename T>
inline sf_parameter cast_to_sf_parameter(T param) {
  return static_cast<sf_parameter>(param);
}

template <typename T>
inline sf_pair_t make_sf_parameter(T id, const sf_value_t value) {
  return {static_cast<sf_id_t>(id), value};
}

}  // namespace mh2c

#endif  // MH2C_FRAME_SETTINGS_FRAME_IPP_

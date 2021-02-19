// Copyright (c) 2021, k-noya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#ifndef MH2C_FRAME_ERROR_CODES_H_
#define MH2C_FRAME_ERROR_CODES_H_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7540#section-7
enum class error_codes : uint32_t {
  NO_ERROR,
  PROTOCOL_ERROR,
  INTERNAL_ERRPR,
  FLOW_CONTROL_ERROR,
  SETTINGS_TIMEOUT,
  STREAM_CLOSED,
  FRAME_SIZE_ERROR,
  REFUSED_STREAM,
  CANCEL,
  COMPRESSION_ERROR,
  CONNECT_ERROR,
  ENHANCE_YOUR_CALM,
  INADEQUATE_SECURITY,
  HTTP_1_1_REQUIRED,
  UNKNOWN_ERROR,
};

const std::unordered_map<error_codes, std::string> error_codes_str_map{
    {error_codes::NO_ERROR, "NO_ERROR"},
    {error_codes::PROTOCOL_ERROR, "PROTOCOL_ERROR"},
    {error_codes::INTERNAL_ERRPR, "INTERNAL_ERRPR"},
    {error_codes::FLOW_CONTROL_ERROR, "FLOW_CONTROL_ERROR"},
    {error_codes::SETTINGS_TIMEOUT, "SETTINGS_TIMEOUT"},
    {error_codes::STREAM_CLOSED, "STREAM_CLOSED"},
    {error_codes::FRAME_SIZE_ERROR, "FRAME_SIZE_ERROR"},
    {error_codes::REFUSED_STREAM, "REFUSED_STREAM"},
    {error_codes::CANCEL, "CANCEL"},
    {error_codes::COMPRESSION_ERROR, "COMPRESSION_ERROR"},
    {error_codes::CONNECT_ERROR, "CONNECT_ERROR"},
    {error_codes::ENHANCE_YOUR_CALM, "ENHANCE_YOUR_CALM"},
    {error_codes::INADEQUATE_SECURITY, "INADEQUATE_SECURITY"},
    {error_codes::HTTP_1_1_REQUIRED, "HTTP_1_1_REQUIRED"},
    {error_codes::UNKNOWN_ERROR, "UNKNOWN_ERROR"},
};

}  // namespace mh2c

#endif  // MH2C_FRAME_ERROR_CODES_H_

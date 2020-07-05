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

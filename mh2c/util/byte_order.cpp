// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#include "mh2c/util/byte_order.h"

#include <arpa/inet.h>

#include <cstdint>

namespace mh2c {

uint16_t cvt_net2host(uint16_t data) { return ntohs(data); }

uint32_t cvt_net2host(uint32_t data) { return ntohl(data); }

uint16_t cvt_host2net(uint16_t data) { return htons(data); }

uint32_t cvt_host2net(uint32_t data) { return htonl(data); }

}  // namespace mh2c

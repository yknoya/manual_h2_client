// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE
#include "mh2c/hpack/static_table_definition.h"

#include "mh2c/hpack/header_type.h"

namespace mh2c {

// cf. https://tools.ietf.org/html/rfc7541#appendix-A
const header_table_t static_table_entries{
    {1u, {":authority", ""}},
    {2u, {":method", "GET"}},
    {3u, {":method", "POST"}},
    {4u, {":path", "/"}},
    {5u, {":path", "/index.html"}},
    {6u, {":scheme", "http"}},
    {7u, {":scheme", "https"}},
    {8u, {":status", "200"}},
    {9u, {":status", "204"}},
    {10u, {":status", "206"}},
    {11u, {":status", "304"}},
    {12u, {":status", "400"}},
    {13u, {":status", "404"}},
    {14u, {":status", "500"}},
    {15u, {"accept-charset", ""}},
    {16u, {"accept-encoding", "gzip, deflate"}},
    {17u, {"accept-language", ""}},
    {18u, {"accept-ranges", ""}},
    {19u, {"accept", ""}},
    {20u, {"accept-control-allow-origin", ""}},
    {21u, {"age", ""}},
    {22u, {"allow", ""}},
    {23u, {"authorization", ""}},
    {24u, {"cache-control", ""}},
    {25u, {"content-disposition", ""}},
    {26u, {"content-encoding", ""}},
    {27u, {"content-language", ""}},
    {28u, {"content-length", ""}},
    {29u, {"content-location", ""}},
    {30u, {"content-range", ""}},
    {31u, {"content-type", ""}},
    {32u, {"cookie", ""}},
    {33u, {"date", ""}},
    {34u, {"etag", ""}},
    {35u, {"expect", ""}},
    {36u, {"expires", ""}},
    {37u, {"from", ""}},
    {38u, {"host", ""}},
    {39u, {"if-match", ""}},
    {40u, {"if-modified-since", ""}},
    {41u, {"if-none-match", ""}},
    {42u, {"if-range", ""}},
    {43u, {"if-unmodified-since", ""}},
    {44u, {"last-modified", ""}},
    {45u, {"link", ""}},
    {46u, {"location", ""}},
    {47u, {"max-forwards", ""}},
    {48u, {"proxy-authenticate", ""}},
    {49u, {"proxy-authorization", ""}},
    {50u, {"range", ""}},
    {51u, {"referer", ""}},
    {52u, {"refresh", ""}},
    {53u, {"retry-after", ""}},
    {54u, {"server", ""}},
    {55u, {"set-cookie", ""}},
    {56u, {"strict-transport-security", ""}},
    {57u, {"transfer-encoding", ""}},
    {58u, {"user-agent", ""}},
    {59u, {"vary", ""}},
    {60u, {"via", ""}},
    {61u, {"www-authenticate", ""}},
};

// This is used for header encoding
const reverse_header_table_t reverse_static_table_entries{
    {{":authority", ""}, 1u},
    {{":method", "GET"}, 2u},
    {{":method", "POST"}, 3u},
    {{":path", "/"}, 4u},
    {{":path", "/index.html"}, 5u},
    {{":scheme", "http"}, 6u},
    {{":scheme", "https"}, 7u},
    {{":status", "200"}, 8u},
    {{":status", "204"}, 9u},
    {{":status", "206"}, 10u},
    {{":status", "304"}, 11u},
    {{":status", "400"}, 12u},
    {{":status", "404"}, 13u},
    {{":status", "500"}, 14u},
    {{"accept-charset", ""}, 15u},
    {{"accept-encoding", "gzip, deflate"}, 16u},
    {{"accept-language", ""}, 17u},
    {{"accept-ranges", ""}, 18u},
    {{"accept", ""}, 19u},
    {{"accept-control-allow-origin", ""}, 20u},
    {{"age", ""}, 21u},
    {{"allow", ""}, 22u},
    {{"authorization", ""}, 23u},
    {{"cache-control", ""}, 24u},
    {{"content-disposition", ""}, 25u},
    {{"content-encoding", ""}, 26u},
    {{"content-language", ""}, 27u},
    {{"content-length", ""}, 28u},
    {{"content-location", ""}, 29u},
    {{"content-range", ""}, 30u},
    {{"content-type", ""}, 31u},
    {{"cookie", ""}, 32u},
    {{"date", ""}, 33u},
    {{"etag", ""}, 34u},
    {{"expect", ""}, 35u},
    {{"expires", ""}, 36u},
    {{"from", ""}, 37u},
    {{"host", ""}, 38u},
    {{"if-match", ""}, 39u},
    {{"if-modified-since", ""}, 40u},
    {{"if-none-match", ""}, 41u},
    {{"if-range", ""}, 42u},
    {{"if-unmodified-since", ""}, 43u},
    {{"last-modified", ""}, 44u},
    {{"link", ""}, 45u},
    {{"location", ""}, 46u},
    {{"max-forwards", ""}, 47u},
    {{"proxy-authenticate", ""}, 48u},
    {{"proxy-authorization", ""}, 49u},
    {{"range", ""}, 50u},
    {{"referer", ""}, 51u},
    {{"refresh", ""}, 52u},
    {{"retry-after", ""}, 53u},
    {{"server", ""}, 54u},
    {{"set-cookie", ""}, 55u},
    {{"strict-transport-security", ""}, 56u},
    {{"transfer-encoding", ""}, 57u},
    {{"user-agent", ""}, 58u},
    {{"vary", ""}, 59u},
    {{"via", ""}, 60u},
    {{"www-authenticate", ""}, 61u},
};

}  // namespace mh2c

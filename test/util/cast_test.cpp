// Copyright (c) 2021, yknoya
// Distributed under the BSD 3-Clause License.
// See accompanying file LICENSE.
#include "mh2c/util/cast.h"

#include <gtest/gtest.h>

#include "mh2c/frame/frame_type_registry.h"
#include "mh2c/hpack/header_type.h"

TEST(cast_test, underlying_cast_converts_frame_type_registry) {
  EXPECT_EQ(0x08,
            mh2c::underlying_cast(mh2c::frame_type_registry::WINDOW_UPDATE));
}

TEST(cast_test, underlying_cast_converts_header_prefix_pattern) {
  EXPECT_EQ(0x10,
            mh2c::underlying_cast(mh2c::header_prefix_pattern::NEVER_INDEXED));
}

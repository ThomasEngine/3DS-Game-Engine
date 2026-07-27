#pragma once
#include "sprite.h"

namespace PlayerAnims {
    constexpr Animation IDLE { 0,  11, 0.10f, true };
    constexpr Animation RUN  { 11, 12, 0.06f, true };
    constexpr Animation JUMP { 23, 1,  0.10f, false };
    constexpr Animation FALL { 24, 1,  0.10f, false };
}
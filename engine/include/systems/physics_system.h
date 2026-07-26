#pragma once
#include "ecs.h"

namespace systems {
    void update_velocity(ECSWorld& world, float dt, const EngineSettings& settings);
    void update_gravity(ECSWorld& world, float dt, const EngineSettings& settings);
}
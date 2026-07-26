#pragma once
#include "ecs.h"
#include "spatial_grid.h"
#include "functional"

struct CollisionEvent {
public:
    Entity a;
    Entity b;
};

using WalkableCallback = std::function<bool(int x, int y)>;
using CollisionCallback = std::function<void(CollisionEvent event, ECSWorld& world)>;

namespace systems {
    void collision_update(ECSWorld& world, WalkableCallback isWalkable, CollisionCallback onEntityCollision, float dt);
}


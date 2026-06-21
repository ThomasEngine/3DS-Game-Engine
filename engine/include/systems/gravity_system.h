#pragma once
#include "ecs.h"
#include "settings.h"

void gravity_system_update(ECSWorld& world, float dt) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isvalid(e)) continue;
        if (!world.hasComponent(e, COMP_GRAVITY | COMP_VELOCITY)) continue;

        if (!world.gravity[e].grounded) {
            world.velocity[e].dy += GRAVITY * world.gravity[e].weight * dt;

            if (world.velocity[e].dy > MAX_FALL_SPEED)
                world.velocity[e].dy = MAX_FALL_SPEED;
        }
    }
}

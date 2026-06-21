#pragma once
#include "ecs.h"

void update_physics_system(ECSWorld& world, float dt) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

        // Apply physics
        PositionComponent& position = world.position[e];
        VelocityComponent& velocity = world.velocity[e];

        position.x += velocity.dx * dt;
        position.y += velocity.dx * dt;
    }
}
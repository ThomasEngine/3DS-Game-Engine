#pragma once
#include "ecs.h"
#include <cmath>
#include "settings.h"

static constexpr float GROUND_FRICTION = BFRICTION ? 0.5f : 0.0f; // if friction is turned on set it to 0.5 if its off don't use it

void update_physics_system(ECSWorld& world, float dt) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

        // Apply physics
        PositionComponent& position = world.position[e];
        VelocityComponent& velocity = world.velocity[e];

        position.x += velocity.dx * dt;
        position.y += velocity.dy * dt;

        velocity.dx = velocity.dx * GROUND_FRICTION;
        velocity.dy = velocity.dy * GROUND_FRICTION;

        if (velocity.dx != 0.0f && std::abs(velocity.dx) < 0.001f) velocity.dx = 0.0f;
        if (velocity.dy != 0.0f && std::abs(velocity.dy) < 0.001f) velocity.dy = 0.0f;
    }
}
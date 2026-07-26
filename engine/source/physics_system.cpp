#include "physics_system.h"

#include <cmath>

#include "engine_settings.h"


namespace systems {
    void update_velocity(ECSWorld& world, float dt, const EngineSettings& settings) {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

            // Apply physics
            PositionComponent& position = world.position[e];
            VelocityComponent& velocity = world.velocity[e];

            position.x += velocity.dx * dt;
            position.y += velocity.dy * dt;


            if (settings.useFriction) {

                const float& friction = settings.friction;

                velocity.dx = pow(settings.friction, dt);
                velocity.dy = pow(settings.friction, dt);
            }


            if (velocity.dx != 0.0f && std::abs(velocity.dx) < 0.001f) velocity.dx = 0.0f;
            if (velocity.dy != 0.0f && std::abs(velocity.dy) < 0.001f) velocity.dy = 0.0f;
        }
    }

    void update_gravity(ECSWorld& world, float dt, const EngineSettings& settings) {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_GRAVITY | COMP_VELOCITY)) continue;

            if (!world.gravity[e].grounded) {
                world.velocity[e].dy += settings.gravity * world.gravity[e].weight * dt;

                if (world.velocity[e].dy > settings.maxFallSpeed)
                    world.velocity[e].dy = settings.maxFallSpeed;
            }
        }
    }
}

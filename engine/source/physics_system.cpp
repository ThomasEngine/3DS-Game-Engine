#include "physics_system.h"

#include <algorithm>
#include <cmath>

#include "engine_settings.h"


namespace systems {
    void update_velocity(ECSWorld &world, float dt, const EngineSettings &settings) {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

            vec2 &position = world.position[e].pos;
            vec2 &velocity = world.velocity[e].dir;

            position += velocity * dt;

            if (settings.useFriction) {
                // velocity.dx *= pow(settings.friction, dt);
                // ground friction
                if (!world.hasComponent(e, COMP_GRAVITY)) {
                    velocity *= pow(settings.frictionGround, dt);
                } else {
                    // on ground
                    if (world.gravity[e].grounded) {
                        velocity.x *= pow(settings.frictionGround, dt);
                    } else {
                        velocity.y *= pow(settings.frictionAir, dt);
                    }
                }
            }
            if (std::abs(velocity.x) < 0.001f) velocity.x = 0.0f;
        }
    }

    void update_gravity(ECSWorld &world, float dt, const EngineSettings &settings) {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_GRAVITY | COMP_VELOCITY)) continue;

            vec2& velocity = world.velocity[e].dir;

            if (!world.gravity[e].grounded) {
                velocity.y += settings.gravity * world.gravity[e].weight * dt;
                if (velocity.y > settings.maxFallSpeed)
                    velocity.y = settings.maxFallSpeed;
            }
        }
    }
}

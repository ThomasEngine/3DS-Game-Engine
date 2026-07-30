#include "physics_system.h"

#include <algorithm>
#include <cmath>

#include "engine_settings.h"


namespace systems {
    void update_velocity(ECSWorld &world, float dt, const EngineSettings &settings) {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

            PositionComponent &position = world.position[e];
            VelocityComponent &velocity = world.velocity[e];

            position.x += velocity.dx * dt;
            position.y += velocity.dy * dt;

            if (settings.useFriction) {
                // velocity.dx *= pow(settings.friction, dt);
                // ground friction
                if (!world.hasComponent(e, COMP_GRAVITY)) {
                    velocity.dx *= settings.frictionGround;
                    velocity.dy *= settings.frictionGround;
                } else {
                    // on ground
                    if (world.gravity[e].grounded) {
                        velocity.dx *= settings.frictionGround;
                    } else {
                        velocity.dy *= settings.frictionAir;
                    }
                }
            }
            if (std::abs(velocity.dx) < 0.001f) velocity.dx = 0.0f;
        }
    }

    void update_gravity(ECSWorld &world, float dt, const EngineSettings &settings) {
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

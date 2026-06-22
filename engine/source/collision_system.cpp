#include "systems/collision_system.h"

#include "ecs.h"
#include "spatial_grid.h"

#include "cmath"

#include "sprite.h" // only for debugging remove when not


void collision_system_update(ECSWorld& world, SpatialGrid& grid, WalkableCallback isWalkable, CollisionCallback onEntityCollision, float dt)
{
    grid.clear();
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_POSITION)) continue;
        grid.insert(e, world.position[e].x, world.position[e].y);
    }

    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

        // predict next position
        float newX = world.position[e].x + world.velocity[e].dx * dt;
        float newY = world.position[e].y + world.velocity[e].dy * dt;

        // round x and y for lookup
        const int roundedX = int(std::round(newX));
        const int roundedY = int(std::round(newY));

        world.sprite[e].sprite.color = C2D_Color32(0,255,0,255);
    auto toTile = [](float f) { return std::round(f);};
    auto tileCenter = [](float f) { return std::floor(f) + 0.5f; };

    if (isWalkable) {
        const float HALF = 0.47f;
        const float SNAP = 5;

        // x movement
        if (world.velocity[e].dx != 0.0f) {
            float checkX = newX + (world.velocity[e].dx > 0 ? HALF : -HALF);
            bool topBlocked    = !isWalkable(toTile(checkX), toTile(world.position[e].y - HALF));
            bool bottomBlocked = !isWalkable(toTile(checkX), toTile(world.position[e].y + HALF));

            if (topBlocked && bottomBlocked) {
                world.velocity[e].dx = 0.0f;
            } else if (topBlocked) {
                world.velocity[e].dx = 0.0f;
                if (isWalkable(toTile(world.position[e].x), toTile(world.position[e].y + HALF + 0.1f))) {
                    float target = tileCenter(world.position[e].y) + 1.0f;
                    float diff = target - world.position[e].y;
                    float move = (diff > 0 ? SNAP : -SNAP);
                    if (std::abs(move * dt) > std::abs(diff)) {
                        world.position[e].y = target;
                        world.velocity[e].dy = 0.0f;
                    } else {
                        world.velocity[e].dy = move;
                    }
                }
            } else if (bottomBlocked) {
                world.velocity[e].dx = 0.0f;
                if (isWalkable(toTile(world.position[e].x), toTile(world.position[e].y - HALF - 0.1f))) {
                    float target = tileCenter(world.position[e].y) - 1.0f;
                    float diff = target - world.position[e].y;
                    float move = (diff > 0 ? SNAP : -SNAP);
                    if (std::abs(move * dt) > std::abs(diff)) {
                        world.position[e].y = target;
                        world.velocity[e].dy = 0.0f;
                    } else {
                        world.velocity[e].dy = move;
                    }
                }
            }
        }

        // y movement
        if (world.velocity[e].dy != 0.0f) {
            float checkY = newY + (world.velocity[e].dy > 0 ? HALF : -HALF);
            bool leftBlocked  = !isWalkable(toTile(world.position[e].x - HALF), toTile(checkY));
            bool rightBlocked = !isWalkable(toTile(world.position[e].x + HALF), toTile(checkY));

            if (leftBlocked && rightBlocked) {
                world.velocity[e].dy = 0.0f;
            } else if (leftBlocked) {
                world.velocity[e].dy = 0.0f;
                if (isWalkable(toTile(world.position[e].x + HALF + 0.1f), toTile(world.position[e].y))) {
                    float target = tileCenter(world.position[e].x) + 1.0f;
                    float diff = target - world.position[e].x;
                    float move = (diff > 0 ? SNAP : -SNAP);
                    if (std::abs(move * dt) > std::abs(diff)) {
                        world.position[e].x = target;
                        world.velocity[e].dx = 0.0f;
                    } else {
                        world.velocity[e].dx = move;
                    }
                }
            } else if (rightBlocked) {
                world.velocity[e].dy = 0.0f;
                if (isWalkable(toTile(world.position[e].x - HALF - 0.1f), toTile(world.position[e].y))) {
                    float target = tileCenter(world.position[e].x) - 1.0f;
                    float diff = target - world.position[e].x;
                    float move = (diff > 0 ? SNAP : -SNAP);
                    if (std::abs(move * dt) > std::abs(diff)) {
                        world.position[e].x = target;
                        world.velocity[e].dx = 0.0f;
                    } else {
                        world.velocity[e].dx = move;
                    }
                }
            }
        }
    }

        // entity collision
        if (onEntityCollision) {
            int tx = (int)world.position[e].x;
            int ty = (int)world.position[e].y;

            Entity neighbours[5] = {
                grid.getAt(tx,     ty),
                grid.getAt(tx + 1, ty),
                grid.getAt(tx - 1, ty),
                grid.getAt(tx,     ty + 1),
                grid.getAt(tx,     ty - 1),
            };

            for (Entity other : neighbours) {
                if (other == INVALID_ENTITY) continue;
                if (other == e) continue;
                if (other < e) continue;
                onEntityCollision({ e, other }, world);
            }
        }
    }
}
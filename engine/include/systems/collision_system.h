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

void collision_system_update(ECSWorld& world, SpatialGrid& grid, WalkableCallback isWalkable, CollisionCallback onEntityCollision) {
    grid.clear();
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_POSITION)) continue;
        grid.insert(e, world.position[e].x, world.position[e].y);
    }

    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

        float newX = world.position[e].x + world.velocity[e].dx;
        float newY = world.position[e].y + world.velocity[e].dy;

        // tile collision
        if (isWalkable) {
            if (!isWalkable((int)newX, (int)world.position[e].y)) {
                world.velocity[e].dx = 0.0f;
                newX = world.position[e].x;
            }

            if (!isWalkable((int)world.position[e].x, (int)newY)) {
                world.velocity[e].dy = 0.0f;
                newY = world.position[e].y;
            }
        }

        world.position[e].x = newX;
        world.position[e].y = newY;

        // entity collision — check neighbours in spatial grid
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
                if (other < e) continue; // avoid duplicate pairs

                onEntityCollision({ e, other }, world);
            }
        }
    }
}

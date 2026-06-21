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

        auto toTile = [](float f) { return (int)std::round(f); };

        if (isWalkable) {
            const float HALF = 0.48f;

            if (world.velocity[e].dx != 0.0f) {
                float checkX = newX + (world.velocity[e].dx > 0 ? HALF : -HALF);
                if (!isWalkable(toTile(checkX), toTile(world.position[e].y - HALF)) ||
                    !isWalkable(toTile(checkX), toTile(world.position[e].y + HALF))) {
                    world.velocity[e].dx = 0.0f;
                    }
            }

            if (world.velocity[e].dy != 0.0f) {
                float checkY = newY + (world.velocity[e].dy > 0 ? HALF : -HALF);
                if (!isWalkable(toTile(world.position[e].x - HALF), toTile(checkY)) ||
                    !isWalkable(toTile(world.position[e].x + HALF), toTile(checkY))) {
                    world.velocity[e].dy = 0.0f;
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
#pragma once
#include "ecs.h"
#include "settings.h"

void update_animation_system(ECSWorld& world, float dt) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_SPRITE | COMP_POSITION)) continue;

        // advance animation
        world.sprite[e].sprite.update(dt);

        // sync position to sprite
        float px = (world.position[e].x + 0.5) * TILE_SIZE_PX * TILE_SCALE;
        float py = (world.position[e].y + 0.5) * TILE_SIZE_PX * TILE_SCALE;
        world.sprite[e].sprite.setPos(px, py);
    }
}
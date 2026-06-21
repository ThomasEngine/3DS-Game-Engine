#pragma once
#include "ecs.h"

void update_animation_system(ECSWorld& world, float dt) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_SPRITE | COMP_POSITION)) continue;

        // sync position to sprite
        float px = world.position[e].x * 16.0f;
        float py = world.position[e].y * 16.0f;
        world.sprite[e].sprite.setPos(px, py);

        // advance animation
        world.sprite[e].sprite.update(dt);
    }
}
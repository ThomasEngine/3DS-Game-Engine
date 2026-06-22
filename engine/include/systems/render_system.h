#pragma once
#include "ecs.h"
#include "renderer.h"
#include "sprite.h"





void render_system_draw(ECSWorld& world, Renderer& renderer, RenderLayer layer) {
    Camera& cam = renderer.getCamera();

    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_SPRITE)) continue;
        if (world.sprite[e].layer != layer) continue;

        Sprite& sprite = world.sprite[e].sprite;

        float px = (world.position[e].x - cam.x + 0.5) * TILE_SIZE_PX * TILE_SCALE;
        float py = (world.position[e].y - cam.y + 0.5) * TILE_SIZE_PX * TILE_SCALE;
        sprite.setPos(px, py);
        sprite.draw();
    }
}
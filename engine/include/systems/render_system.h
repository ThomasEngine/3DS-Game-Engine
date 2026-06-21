#pragma once
#include "ecs.h"
#include "renderer.h"
#include "sprite.h"





void render_system_draw(ECSWorld& world, Renderer& renderer, RenderLayer layer) {
    for (Entity e = 0; e < MAX_ENTITIES; e++) {
        if (!world.isValid(e)) continue;
        if (!world.hasComponent(e, COMP_SPRITE)) continue;
        if (world.sprite[e].layer != layer) continue;

        Sprite& sprite = world.sprite[e].sprite;
        //renderer.drawRect(sprite.getX(), sprite.getY(), sprite.getWidth(), sprite.getHeight(), C2D_Color32(255, 0, 0, 255));
        sprite.draw();
    }
}
#include "render_system.h"

#include "sprite.h"
#include "constants.h"



namespace systems {
    void render_draw(ECSWorld& world, Renderer& renderer, RenderLayer layer, const EngineSettings& settings) {
        Camera& cam = renderer.getCamera();

        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_SPRITE)) continue;
            if (world.sprite[e].layer != layer) continue;

            Sprite& sprite = world.sprite[e].sprite;

            float px = (world.position[e].x - cam.x + 0.5) * settings.tileSizePx * settings.tileScale;
            float py = (world.position[e].y - cam.y + 0.5) * settings.tileSizePx * settings.tileScale;
            sprite.setPos(px, py);
            sprite.draw();
        }
    }
}

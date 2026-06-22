#include "level_renderer.h"
#include "renderer.h"
#include "settings.h"


void draw_level(int width, int height, const Camera &camera, const TileSpriteCallback &getSprite) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            C2D_Image img = getSprite(x, y);
            float px = (x - camera.x) * TILE_SIZE_PX * TILE_SCALE;
            float py = (y - camera.y) * TILE_SIZE_PX * TILE_SCALE;
            C2D_DrawImageAt(img, px, py, 0, nullptr, TILE_SCALE, TILE_SCALE);
        }
    }
}
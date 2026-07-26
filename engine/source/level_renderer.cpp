#include "level_renderer.h"
#include "renderer.h"
#include <cmath>
#include "tiled_loader.h"

void draw_level(int width, int height, const Camera &camera, const TileSpriteCallback &getSprite, const EngineSettings &settings) {
    const float tile_scale = settings.tileScale;
    const float tile_size = settings.tileSizePx;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            C2D_Image img = getSprite(x, y);
            float px = std::round((x - camera.x) * tile_size * tile_scale);
            float py = std::round((y - camera.y) * tile_size * tile_scale);
            C2D_DrawImageAt(img, px, py, 0, nullptr, tile_scale, tile_scale);
        }
    }
}

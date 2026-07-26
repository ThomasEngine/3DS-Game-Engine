#include "tiled_render.h"
#include <cmath>

static constexpr uint32_t FLIP_FLAGS = 0xE0000000;  // top 3 bits

int tile_index_from_gid(uint32_t gid) {
    uint32_t clean = gid & ~FLIP_FLAGS;
    if (clean == 0) return -1;
    return (int)(clean - 1);   // Tiled GIDs start at 1
}

void draw_tiled_map(const TiledMap &map, const Camera &cam, C2D_SpriteSheet tileSheet, const EngineSettings &settings) {
    if (!tileSheet) return;
    int sheetCount = C2D_SpriteSheetCount(tileSheet);
    if (sheetCount == 0) return;

    float tileScale = settings.tileScale;

    float tw = map.tileWidth;
    float th = map.tileHeight;

    // how many tiles fit on screen
    float pxPerTile = map.tileWidth * tileScale;
    int screenTilesW = (int)(cam.SCREEN_WIDTH / pxPerTile) + 2;  // +2 for partial edges
    int screenTilesH = (int)(cam.SCREEN_HEIGHT / pxPerTile) + 2;

    for (const auto& layer : map.layers) {
        // first visible tile (top-left), based on camera
        int startX = (int)(cam.x * layer.parallax);
        int startY = (int)(cam.y * layer.parallax);

        int endX = startX + screenTilesW;
        int endY = startY + screenTilesH;

        // clamp to layer bounds
        if (startX < 0) startX = 0;
        if (startY < 0) startY = 0;
        if (endX > layer.width)  endX = layer.width;
        if (endY > layer.height) endY = layer.height;

        for (int y = startY; y < endY; y++) {
            for (int x = startX; x < endX; x++) {
                int idx = y * layer.width + x;
                int sprite = tile_index_from_gid(layer.tiles[idx]);
                if (sprite < 0) continue;

                if (sprite == -1 || sprite == 0 || sprite >= sheetCount) continue;

                C2D_Image img = C2D_SpriteSheetGetImage(tileSheet, sprite);
                float px = std::round((x - cam.x * layer.parallax) * tw * tileScale);
                float py = std::round((y - cam.y * layer.parallax) * th * tileScale);
                C2D_DrawImageAt(img, px, py, 0, nullptr, tileScale, tileScale);

            }
        }
    }
}

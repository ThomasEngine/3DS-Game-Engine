#pragma once
#include <functional>
#include <citro2d.h>
#include <citro3d.h>
#include "settings.h"

using TileSpriteCallback = std::function<C2D_Image(int x, int y)>;

const int TILE_SIZE = TILE_SIZE_PX;


void draw_level(int width, int height, TileSpriteCallback getSprite) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            C2D_Image img = getSprite(x, y);
            C2D_DrawImageAt(img, x * TILE_SIZE * TILE_SCALE, y * TILE_SIZE * TILE_SCALE,  0, nullptr, TILE_SCALE, TILE_SCALE);
        }
    }
}
// class LevelRenderer {
// public:
//     LevelRenderer() = default;
//     ~LevelRenderer();
//
//     void bake(int width, int height, TileSpriteCallback getSprite);
//     void draw();
//
//     bool isBaked() const { return baked; }
//
// private:
//     C3D_RenderTarget* renderTarget = nullptr;
//     bool baked = false;
// };
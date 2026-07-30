#include "background.h"

#include <cmath>

bool load_background(Background &bg, const char *path, float parallax, float scale, bool flipX, bool flipY) {
    bg.sheet = C2D_SpriteSheetLoad(path);
    if (!bg.sheet) return false;
    bg.image = C2D_SpriteSheetGetImage(bg.sheet, 0);
    bg.parallax = parallax;
    bg.scale = scale;
    bg.tileX = flipX;
    bg.tileY = flipY;
    return true;
}

void free_background(Background &bg) {
    if (bg.sheet) {
        C2D_SpriteSheetFree(bg.sheet);
        bg.sheet = nullptr;
    }
}

void draw_parallax_backgrounds(const std::vector<Background> &backgrounds, const Camera &camera,
                               const EngineSettings &settings) {
    const float SCREEN_W = 400.0f;
    const float SCREEN_H = 240.0f;

    // convert camera from tiles to pixels
    const float pxPerTile = settings.tileSizePx * settings.tileScale;
    const float camPxX = camera.x * pxPerTile;
    const float camPxY = camera.y * pxPerTile;

    for (const auto &bg: backgrounds) {
        float imgW = bg.image.subtex->width * bg.scale;
        float imgH = bg.image.subtex->height * bg.scale;

        float offX = camera.x * bg.parallax;
        float offY = camera.y * bg.parallax;

        if (bg.tileX) {
            int startX = -(int) std::floor(fmodf(offX, (float) imgW));
            if (startX > 0) startX -= imgW;

            if (bg.tileY) {
                float startY = -fmodf(offY, imgH);
                if (startY > 0) startY -= imgH;
                for (float x = startX; x < SCREEN_W; x += imgW) {
                    for (float y = startY; y < SCREEN_H; y += imgH) {
                        C2D_DrawImageAt(bg.image, std::round(x), std::round(y), 0,
                                        nullptr, bg.scale, bg.scale);
                    }
                }
            } else {
                float y = -offY;
                for (float x = startX; x < SCREEN_W; x += imgW) {
                    C2D_DrawImageAt(bg.image, float(x), float(y), 0,
                                    nullptr, bg.scale, bg.scale);
                }
            }
        } else {
            C2D_DrawImageAt(bg.image, (float) std::round(-offX), (float) std::round(-offY), 0,
                            nullptr, bg.scale, bg.scale);
        }
    }
}

#pragma once
#include <vector>

#include "engine_settings.h"
#include "Renderer.h"

struct Background {
    C2D_SpriteSheet sheet = nullptr;
    C2D_Image image;
    float parallaxX = 1.0f;
    float parallaxY = 1.0f;
    float scale = 1.0f;
    bool tileX = true; // infinite scrolling background
    bool tileY = false;
};
namespace background {
    bool load(Background& bg, const char *path, float parallax, float scale = 1.0f, bool flipX = true, bool flipY = false);
    bool free (Background& bg);
    void draw_parallax(const std::vector<Background> &backgrounds, const Camera &camera, const EngineSettings& settings);
}
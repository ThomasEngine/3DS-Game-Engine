#pragma once
#include <vector>

#include "engine_settings.h"
#include "Renderer.h"

// Background definition
struct Background {
    C2D_SpriteSheet sheet = nullptr;
    C2D_Image image;
    float parallax = 1.0f;
    float scale = 1.0f;
    bool tileX = true; // infinite scrolling background
    bool tileY = false;
};

// Load a background Image into a background
bool load_background(Background &bg, const char *path, float parallax, float scale = 1.0f, bool flipX = true,
                     bool flipY = false);

// Free a backgrounds sheet
void free_background(Background &bg);

// Draw a list of backgrounds with parallax
void draw_parallax_backgrounds(const std::vector<Background> &backgrounds, const Camera &camera,
                               const EngineSettings &settings);

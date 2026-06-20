#pragma once
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>

struct GraphicsAssets {
    C2D_SpriteSheet sprites;
    C2D_SpriteSheet tiles;
    C2D_SpriteSheet effects;
};

GraphicsAssets* graphics_load_assets();
void graphics_free_assets(GraphicsAssets* assets);
C2D_Image graphics_get_sprite(GraphicsAssets* assets, const char* sheet, int idx);
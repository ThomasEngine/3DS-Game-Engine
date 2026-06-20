#pragma once
#include <citro2d.h>
#include <citro3d.h>

typedef struct {
    C2D_SpriteSheet sprites;
    C2D_SpriteSheet tiles;
    C2D_SpriteSheet effects;
} GraphicsAssets;

GraphicsAssets* graphics_load_assets(void);
void graphics_free_assets(GraphicsAssets* assets);

C2D_Image graphics_get_sprite(GraphicsAssets* assets, const char* sheet, int idx);
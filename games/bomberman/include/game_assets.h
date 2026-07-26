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

// Game specific sprite indices
enum SpriteIndex {
    SPR_PLAYER    = 0,
    SPR_BOMB      = 1,
    SPR_EXPLOSION = 2,
};

enum TileIndex {
    TILE_IDX_FLOOR      = 0,
    TILE_IDX_WALL       = 1,
    TILE_IDX_BREAKABLE  = 2,
    TILE_IDX_FLOOR_ALT  = 3,
};

// Game specific asset helpers
inline C2D_Image getSprite(GraphicsAssets* assets, SpriteIndex idx) {
    return C2D_SpriteSheetGetImage(assets->sprites, idx);
}

inline C2D_Image getTile(GraphicsAssets* assets, TileIndex idx) {
    return C2D_SpriteSheetGetImage(assets->tiles, idx);
}
#pragma once
#include "graphics.h"

// Game specific sprite indices
enum SpriteIndex {
    SPR_PLAYER    = 0,
    SPR_BOMB      = 1,
    SPR_EXPLOSION = 2,
};

enum TileIndex {
    TILE_FLOOR      = 0,
    TILE_WALL       = 1,
    TILE_BREAKABLE  = 2,
    TILE_FLOOR_ALT  = 3,
};

// Game specific asset helpers
inline C2D_Image getSprite(GraphicsAssets* assets, SpriteIndex idx) {
    return C2D_SpriteSheetGetImage(assets->sprites, idx);
}

inline C2D_Image getTile(GraphicsAssets* assets, TileIndex idx) {
    return C2D_SpriteSheetGetImage(assets->tiles, idx);
}
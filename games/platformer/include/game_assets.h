#pragma once
#include <3ds.h>
#include <atomic>
#include <citro3d.h>
#include <citro2d.h>
#include <vector>
#include "background.h"




struct GraphicsAssets {
    C2D_SpriteSheet tiles;
    C2D_SpriteSheet player;
    std::vector<Background> backgrounds;
};




GraphicsAssets* graphics_load_assets();
inline void graphics_free_assets(GraphicsAssets* assets) {
    if (!assets) return;
    C2D_SpriteSheetFree(assets->player);
    C2D_SpriteSheetFree(assets->tiles);
    delete assets;
}

inline GraphicsAssets* graphics_load_assets() {
    GraphicsAssets* assets = new GraphicsAssets();

    assets->tiles  = C2D_SpriteSheetLoad("romfs:/gfx/tiles.t3x");
    assets->player = C2D_SpriteSheetLoad("romfs:/gfx/player.t3x");

    //
    Background background1;
    load_background(background1, "romfs:/gfx/Background3.t3x", 0.75f, 2.0f);
    assets->backgrounds.push_back(background1);

    Background background2;
    load_background(background2, "romfs:/gfx/Background2.t3x", 0.90f, 2.0f);
    assets->backgrounds.push_back(background2);

    Background background3;
    load_background(background3, "romfs:/gfx/Background1.t3x", 1.f, 2.0f);
    assets->backgrounds.push_back(background3);


    if (!assets->tiles || !assets->player || assets->backgrounds.empty()) {
        delete assets;
        return nullptr;
    }

    return assets;
}


#include "game_assets.h"

GraphicsAssets* graphics_load_assets() {
    GraphicsAssets* assets = new GraphicsAssets();

    assets->sprites = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    assets->tiles   = C2D_SpriteSheetLoad("romfs:/gfx/tiles.t3x");
    assets->effects = C2D_SpriteSheetLoad("romfs:/gfx/effects.t3x");

    if (!assets->sprites || !assets->tiles || !assets->effects) {
        delete assets;
        return nullptr;
    }

    return assets;
}

void graphics_free_assets(GraphicsAssets* assets) {
    if (!assets) return;
    C2D_SpriteSheetFree(assets->sprites);
    C2D_SpriteSheetFree(assets->tiles);
    C2D_SpriteSheetFree(assets->effects);
    delete assets;
}
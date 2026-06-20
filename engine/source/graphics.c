#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

GraphicsAssets* graphics_load_assets(void) {
    GraphicsAssets* assets = malloc(sizeof(GraphicsAssets));
    
    assets->sprites = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    assets->tiles = C2D_SpriteSheetLoad("romfs:/gfx/tiles.t3x");
    assets->effects = C2D_SpriteSheetLoad("romfs:/gfx/effects.t3x");
    
    if (!assets->sprites || !assets->tiles || !assets->effects) {
        printf("Failed to load graphics assets!\n");
        free(assets);
        return NULL;
    }
    
    return assets;
}

void graphics_free_assets(GraphicsAssets* assets) {
    if (!assets) return;
    C2D_SpriteSheetFree(assets->sprites);
    C2D_SpriteSheetFree(assets->tiles);
    C2D_SpriteSheetFree(assets->effects);
    free(assets);
}
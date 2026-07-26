// games/platformer/source/level.cpp
#include "level.h"

#include "tiled_render.h"


bool Level::load(const std::string& path) {
    if (!loadTiledMap(path, map)) {
        return false;
    }

    // dimensions come from the first layer (all layers same size in Tiled)
    if (!map.layers.empty()) {
        width  = map.layers[0].width;
        height = map.layers[0].height;
    }

    // find the collision layer by name
    collisionLayer = nullptr;
    for (const auto& layer : map.layers) {
        if (layer.name == "collision") {
            collisionLayer = &layer;
            break;
        }
    }

    buildCollision();
    return true;
}

void Level::buildCollision() {
    grid.init(width, height);
    grid.clear();
    // the spatial grid tracks entities, not tiles — tiles are checked via isSolid
    // (nothing to insert here; isSolid reads the collision layer directly)
}

bool Level::isSolid(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true;   // out of bounds = solid (blocks movement)
    }
    if (!collisionLayer) {
        return false;  // no collision layer = nothing solid
    }
    int idx = y * width + x;
    if (idx < 0 || idx >= (int)collisionLayer->tiles.size()) {
        return true;
    }
    // non-empty tile in the collision layer = solid
    return tile_index_from_gid(collisionLayer->tiles[idx]) != -1;
}

bool Level::isWalkable(int x, int y) const {
    return !isSolid(x, y);
}
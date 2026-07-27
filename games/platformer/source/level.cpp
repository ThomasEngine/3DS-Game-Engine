// games/platformer/source/level.cpp
#include "level.h"

#include "tiled_render.h"


bool Level::load(const std::string& path) {
    if (!loadTiledMap(path, map)) {
        return false;
    }

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

    return true;
}

bool Level::isSolid(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true;
    }
    if (!collisionLayer) {
        return false;
    }
    int idx = y * width + x;
    if (idx < 0 || idx >= (int)collisionLayer->tiles.size()) {
        return true;
    }

    return tile_index_from_gid(collisionLayer->tiles[idx]) != -1;
}

Vec2 Level::getPlayerStartPos() {
    for (const auto& objectLayers : map.objectLayers) {
        for (const auto& object : objectLayers.objects) {
            if (object.name == "player_start") {
                return Vec2(object.x, object.y);
            }
        }
    }
    return Vec2(-1);
}

bool Level::isWalkable(int x, int y) const {
    return !isSolid(x, y);
}

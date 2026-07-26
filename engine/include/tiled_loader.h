#pragma once
#include <string>
#include <vector>
#include <stdint.h>

// namespace Engine {
    struct TiledLayer {
        std::vector<uint32_t> tiles;
        float parallax = 1.0f;
        std::string name;
        int width, height;
    };

    struct TiledMap {
        std::vector<TiledLayer> layers;
        int tileWidth, tileHeight; // in pixels
        int width, height; // in tiles
    };

    bool loadTiledMap(const std::string& filePath, TiledMap& outMap);
// }


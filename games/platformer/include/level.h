#pragma once
#include "tiled_loader.h"
#include "spatial_grid.h"
#include <string>
#include <vector>

class Level {
public:
    bool load(const std::string& path);

    // collision queries (for collision system)
    bool isWalkable(int x, int y) const;   // true if NOT solid and in bounds
    bool isSolid(int x, int y) const;

    // dimensions (in tiles)
    int getWidth()  const { return width; }
    int getHeight() const { return height; }

    // rendering — access to layers + map
    const std::vector<TiledLayer>& getLayers() const { return map.layers; }
    int getTileWidth()  const { return map.tileWidth; }
    int getTileHeight() const { return map.tileHeight; }
    const TiledMap& getMap() const { return map; }


private:
    TiledMap map;
    int width  = 0;
    int height = 0;

    const TiledLayer* collisionLayer = nullptr;
};
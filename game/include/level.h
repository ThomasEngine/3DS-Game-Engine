#pragma once
#include "spatial_grid.h"

enum TileType {
    TILE_FLOOR,
    TILE_WALL,
    TILE_BREAKABLE,
};

class Level {
public:
    Level();
    ~Level() = default;

    // loading
    bool load(const char* path);
    void loadDefault(); // hardcoded test level

    // tile getings
    TileType getTile(int x, int y) const;
    bool isWalkable(int x, int y) const;
    bool isBreakable(int x, int y) const;
    bool inBounds(int x, int y) const;

    // tile modification
    void setTile(int x, int y, TileType type);
    void destroyTile(int x, int y); // breakable -> floor

    // dimensions
    int getWidth()  const { return width; }
    int getHeight() const { return height; }

    // spatial grid access
    SpatialGrid& getGrid() { return grid; }

    // rendering info
    // returns tile image index for renderer
    int getTileSprite(int x, int y);

private:
    int      width  = 0;
    int      height = 0;
    TileType tiles[SpatialGrid::MAX_W][SpatialGrid::MAX_H];
    SpatialGrid grid;
};
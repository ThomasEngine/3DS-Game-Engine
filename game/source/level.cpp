#include "level.h"


Level::Level() {
    // initialize all tiles to floor
    for (int x = 0; x < SpatialGrid::MAX_W; x++)
        for (int y = 0; y < SpatialGrid::MAX_H; y++)
            tiles[x][y] = TILE_FLOOR;
}

bool Level::inBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

TileType Level::getTile(int x, int y) const {
    if (!inBounds(x, y)) return TILE_WALL;
    return tiles[x][y];
}

bool Level::isWalkable(int x, int y) const {
    if (!inBounds(x, y)) return false;
    return tiles[x][y] == TILE_FLOOR;
}

bool Level::isBreakable(int x, int y) const {
    if (!inBounds(x, y)) return false;
    return tiles[x][y] == TILE_BREAKABLE;
}

void Level::setTile(int x, int y, TileType type) {
    if (!inBounds(x, y)) return;
    tiles[x][y] = type;
}

void Level::destroyTile(int x, int y) {
    if (!isBreakable(x, y)) return;
    tiles[x][y] = TILE_FLOOR;
}

int Level::getTileSprite(int x, int y) {
    switch (getTile(x, y)) {
        case TILE_FLOOR:     return 0;
        case TILE_WALL:      return 1;
        case TILE_BREAKABLE: return 2;
        default:             return 0;
    }
}

void Level::loadDefault() {
    width  = 15;
    height = 13;

    grid.init(width, height);

    // fill with floor
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++) {
            tiles[x][y] = TILE_FLOOR;

        }

    // border walls
    for (int x = 0; x < width; x++) {
        tiles[x][0]          = TILE_WALL;
        tiles[x][height - 1] = TILE_WALL;
    }
    for (int y = 0; y < height; y++) {
        tiles[0][y]         = TILE_WALL;
        tiles[width - 1][y] = TILE_WALL;
    }

    // inner walls — classic Bomberman grid pattern
    for (int x = 2; x < width - 1; x += 2)
        for (int y = 2; y < height - 1; y += 2)
            tiles[x][y] = TILE_WALL;

    // breakable blocks
    for (int x = 1; x < width - 1; x++)
        for (int y = 1; y < height - 1; y++)
            if (tiles[x][y] == TILE_FLOOR)
                if (!(x <= 2 && y <= 2)) // keep player spawn clear
                    tiles[x][y] = TILE_BREAKABLE;
}

bool Level::load(const char* path) {
    // load from implementation here
    loadDefault();
    return true;
}
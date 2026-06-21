#include "spatial_grid.h"

void SpatialGrid::init(int w, int h) {
    width  = w;
    height = h;
    clear();
}

void SpatialGrid::clear() {
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            cells[x][y] = INVALID_ENTITY;
}

void SpatialGrid::insert(Entity e, float tx, float ty) {
    int x = (int)tx;
    int y = (int)ty;
    if (x >= 0 && x < width && y >= 0 && y < height)
        cells[x][y] = e;
}

void SpatialGrid::remove(float tx, float ty) {
    int x = (int)tx;
    int y = (int)ty;
    if (x >= 0 && x < width && y >= 0 && y < height)
        cells[x][y] = INVALID_ENTITY;
}

Entity SpatialGrid::getAt(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return INVALID_ENTITY;
    return cells[x][y];
}

bool SpatialGrid::isEmpty(int x, int y) const {
    return getAt(x, y) == INVALID_ENTITY;
}

bool SpatialGrid::isInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}
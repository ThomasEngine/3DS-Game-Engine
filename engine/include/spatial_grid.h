#pragma once
#include "ecs.h"

class SpatialGrid {
public:
    static const int MAX_W = 32;
    static const int MAX_H = 32;

    void init(int w, int h);

    void clear();

    void insert(Entity e, float tx, float ty);

    void remove(float tx, float ty);

    Entity getAt(int x, int y) const;

    bool isEmpty(int x, int y) const;

    bool isInBounds(int x, int y) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setWidth(int w) { if (w < MAX_W) width = w; }
    void setHeight(int h) { if (h < MAX_H) height = h; }

private:
    Entity cells[MAX_W][MAX_H];
    int width = 0;
    int height = 0;
};

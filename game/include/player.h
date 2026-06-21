#pragma once
#include "ecs.h"
#include "graphics.h"

class Player {
public:
    Player() : entity(INVALID_ENTITY) {};
    ~Player() = default;

    void init(ECSWorld& world, GraphicsAssets* assets, float startX, float startY);
    void destroy(ECSWorld& world);
    void handleInput(ECSWorld& world);

    Entity getEntity() const { return entity; }

private:
    Entity entity;
    float speed = 0.0f;
};

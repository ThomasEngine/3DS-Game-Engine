#pragma once

#include "ecs.h"
#include "game_assets.h"
#include "vec2.h"

// Player wrapper for platformer
class Player {
public:
    Player() : entity(INVALID_ENTITY) {}
    ~Player() = default;

    void init(ECSWorld& world, GraphicsAssets* assets, Vec2 startPos, const EngineSettings& settings);
    void destroy(ECSWorld& world);

    void handleInput(ECSWorld& world);

    Entity getEntity() const { return entity; }

private:
    Entity entity;
    float speed = 0.0f;
};
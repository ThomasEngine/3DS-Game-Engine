#pragma once
#include "core.h"
#include "ecs.h"
#include "player.h"
#include "systems/collision_system.h"

class GraphicsAssets;
class Level;

class GameScene : public BaseScene {
public:
    GameScene() {};
    ~GameScene() = default;

    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void renderTop(Renderer& renderer) override;
    void renderBottom(Renderer& renderer) override;

private:
    void onCollision(CollisionEvent event, ECSWorld& world);

    ECSWorld world;
    GraphicsAssets* assets = nullptr;
    Level* level  = nullptr;
    Player player;
};
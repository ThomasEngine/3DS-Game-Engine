#pragma once
#include "collision_system.h"
#include "core.h"
#include "player.h"

class GraphicsAssets;
class Level;
class MainScene : public BaseScene {
public:
    MainScene() = default;
    ~MainScene() = default;

    void enter() override;
    void exit() override;
    void update(float dt) override;
    void renderTop() override;
    void renderBottom() override;

private:
    void onCollision(CollisionEvent event, ECSWorld& world);

    Player player;
    ECSWorld world;
    Level* level;
    GraphicsAssets* assets;
};
#pragma once
#include "core.h"
#include "player.h"
#include "systems/collision_system.h"

class GraphicsAssets;
class Level;
class GameApp : public IGame {
public:
    GameApp();
    ~GameApp();

    bool init(ECSWorld *world, Renderer &ren) override;
    void update(float deltaTime)   override;
    void render(Renderer& ren)   override;
    void shutdown() override;

private:
    void onCollision(CollisionEvent event, ECSWorld& world);

    ECSWorld* world = nullptr;
    GraphicsAssets* assets = nullptr;
    Level* level = nullptr;
    Player player;
};
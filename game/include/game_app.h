#pragma once
#include "core.h"
#include "player.h"

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
    ECSWorld* world = nullptr;
    GraphicsAssets* assets = nullptr;
    Level* level = nullptr;
    Player player;
};
#pragma once
#include "core.h"
#include "graphics.h"

class GameApp : public IGame {
public:
    GameApp();
    ~GameApp();

    bool init(ECSWorld *world, Renderer &ren) override;
    void update(float deltaTime)   override;
    void render(Renderer& ren)   override;
    void shutdown() override;

private:
    ECSWorld* world;
    GraphicsAssets* assets;
    class Level*    level;
};
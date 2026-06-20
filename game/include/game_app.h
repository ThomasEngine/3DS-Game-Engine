#pragma once
#include "core.h"
#include "graphics.h"

class GameApp : public IGame {
public:
    GameApp();
    ~GameApp();

    bool init()     override;
    void update()   override;
    void render()   override;
    void shutdown() override;

private:
    GraphicsAssets* assets;
    class Level*    level;
};
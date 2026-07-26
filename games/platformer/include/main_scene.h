#pragma once
#include "core.h"

class GraphicsAssets;
class Level;
class MainScene : public BaseScene {
public:
    MainScene() = default;
    ~MainScene() = default;

    void enter() override;
    void exit() override;
    void update(float dt) override;
    void renderTop(Renderer& renderer) override;
    void renderBottom(Renderer& renderer) override;

private:
    Level* level;
    GraphicsAssets* assets;
    Camera camera;
};
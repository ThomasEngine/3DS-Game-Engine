#pragma once
#include "core.h"

class NetworkSystem;
class MainScene : public BaseScene {
public:
    MainScene() = default;
    ~MainScene() = default;

    void enter() override;
    void exit() override;
    void update(float dt) override;
    void renderTop() override {}
    void renderBottom() override;

private:
    NetworkSystem* networkSystem;
};
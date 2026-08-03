#pragma once
#include "core.h"


class MainScene : public BaseScene {
public:
    MainScene() = default;
    ~MainScene() = default;

    void enter() override;
    void exit() override;
    void update(float dt) override;
    void renderTop() override {}
    void renderBottom() override;
};
#pragma once
#include "engine_settings.h"

class BaseScene;
class Renderer;

class Camera;

class SceneManager {
public:
    SceneManager() = default;

    ~SceneManager();

    // request a switch — actual swap happens at start of next frame
    void changeScene(BaseScene *next);

    void update(float dt);

    void renderTop();

    void renderBottom();

    BaseScene *getCurrent() const { return current; }

    void setDefaultSettings(EngineSettings &settings);

    EngineSettings &getSettings() { return settings; }

    void setRenderer(Renderer *ren) { renderer = ren; }

private:
    void applyPendingSwitch();

    BaseScene *current = nullptr;
    BaseScene *pending = nullptr;
    bool hasPending = false;

    EngineSettings engine_default_settings;
    EngineSettings settings;

    Renderer *renderer = nullptr;
};

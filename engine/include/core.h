#pragma once
#include "Renderer.h"
#include "scene_manager.h"
#include "engine_settings.h"
#include "vec2.h"

// Abstract scene interface
class BaseScene {
public:
    virtual ~BaseScene() = default;

    virtual void enter() {}
    virtual void exit() {}
    virtual void update(float dt) = 0;
    virtual void renderTop(Renderer& renderer) = 0;
    virtual void renderBottom(Renderer& renderer) = 0;

    virtual void configurateSettings(EngineSettings& settings) {}

    void setManager(SceneManager* mgr) { manager = mgr; }

protected:
    SceneManager* manager = nullptr;
};

class Engine {
public:
    Engine();
    ~Engine();

    void run();

    void setScene(BaseScene* scene);

    float getDeltaTime() const { return delta_time; }
    bool shouldExit()      const { return !running; }
    void requestExit() { running = false; }
	Renderer& getRenderer() { return renderer; }

    void setDefaultSettings(EngineSettings& settings);

private:
	Renderer renderer;
    SceneManager sceneManager;

    bool   running;
    float  delta_time;
};
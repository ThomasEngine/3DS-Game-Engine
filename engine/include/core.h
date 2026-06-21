#pragma once
#include "Renderer.h"
#include "ecs.h"

// Abstract interface for the game
class IGame {
public:
    virtual ~IGame() = default;
    virtual bool init(ECSWorld *world, Renderer &ren) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(Renderer& ren) = 0;
    virtual void shutdown() = 0;
};

class Engine {
public:
    Engine(IGame* game);
    ~Engine();

    void run();

    float getDeltaTime() const { return delta_time; }
    bool shouldExit()      const { return !running; }
    void requestExit() { running = false; }
	Renderer& getRenderer() { return renderer; }

private:
	Renderer renderer;
    ECSWorld world;

    IGame* game;
    bool   running;
    float  delta_time;
};
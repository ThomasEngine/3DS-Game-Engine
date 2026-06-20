#pragma once
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>

// Abstract interface for the game
class IGame {
public:
    virtual ~IGame() = default;
    virtual bool init() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;
};

class Engine {
public:
    Engine(IGame* game);
    ~Engine();

    void run();

    // Query engine state (for game to use)
    C3D_RenderTarget* getTopTarget()    const { return top; }
    C3D_RenderTarget* getBottomTarget() const { return bottom; }
    float             getDeltaTime()    const { return delta_time; }
    bool              shouldExit()      const { return !running; }
    void              requestExit()           { running = false; }

private:
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
    C2D_TextBuf       text_buf;

    IGame* game;
    bool   running;
    float  delta_time;
};
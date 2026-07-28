#include "core.h"

#include <algorithm>

#include "input.h"
#include <stdio.h>
#include <time.h>

static u64 lastTick = 0;

Engine::Engine() :  running(false), delta_time(0.016f) {
    renderer.init();
    sceneManager.setRenderer(&renderer);
}

Engine::~Engine() {
	renderer.shutdown();
}

void Engine::run() {
    running = true;

    while (aptMainLoop() && running) {
        // delta time
        u64 currentTick = svcGetSystemTick();
        u64 deltaTick = currentTick - lastTick;
        lastTick = currentTick;

        float delta_time = std::clamp((float)deltaTick / SYSCLOCK_ARM11, 0.0f, 0.1f);

        // Input
        hidScanInput();
        u32 keys_down = hidKeysDown();
        u32 keys_held = hidKeysHeld();
        u32 keys_up   = hidKeysUp();
        touchPosition touch;
        hidTouchRead(&touch);

        input_update(keys_down, keys_held, keys_up, touch);

        if (keys_down & KEY_START) {
            running = false;
            break;
        }

        // Game update
        sceneManager.update(delta_time);

        // Rendering
        renderer.beginFrame();
        renderer.beginTopScreen();
        sceneManager.renderTop();
        renderer.beginBottomScreen();
        sceneManager.renderBottom();
        renderer.endFrame();
    }
}

void Engine::setScene(BaseScene* scene) {
    sceneManager.changeScene(scene);
}

void Engine::setDefaultSettings(EngineSettings& settings)
{
    sceneManager.setDefaultSettings(settings);
}
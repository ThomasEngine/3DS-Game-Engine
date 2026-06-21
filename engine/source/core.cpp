#include "core.h"
#include "input.h"
#include <stdio.h>

Engine::Engine(IGame* game) : game(game), running(false), delta_time(0.016f) {
    renderer.init();
}

Engine::~Engine() {
	renderer.shutdown();
}

void Engine::run() {
    if (!game->init(&world, renderer)) {
        printf("Game initialization failed!\n");
        return;
    }

    running = true;

    while (aptMainLoop() && running) {
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
        game->update(delta_time);

        // Rendering
        renderer.beginFrame();
        game->render(renderer);
        renderer.endFrame();
    }

    game->shutdown();
}
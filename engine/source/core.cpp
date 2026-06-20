#include "core.h"
#include "input.h"
#include <stdio.h>

Engine::Engine(IGame* game) : game(game), running(false), delta_time(0.016f) {
    // Initialize graphics
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    //romfsInit();

    // Create render targets
    top    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    text_buf = C2D_TextBufNew(4096);
}

Engine::~Engine() {
    C2D_TextBufDelete(text_buf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
}

void Engine::run() {
    if (!game->init()) {
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
        game->update();

        // Render frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        // Top screen
        C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(top);

        // Bottom screen
        C2D_TargetClear(bottom, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(bottom);

        // Game render
        game->render();

        C3D_FrameEnd(0);
    }

    game->shutdown();
}
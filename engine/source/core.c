#include "core.h"
#include "input.h"
#include <stdlib.h>
#include <stdio.h>

Engine* engine_create(const GameCallbacks* app, void* user_data) {
    Engine* engine = malloc(sizeof(Engine));
    if (!engine) return NULL;
    
    // Initialize graphics
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    romfsInit();
    
    // Create render targets
    engine->top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    engine->bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    
    if (!engine->top || !engine->bottom) {
        free(engine);
        return NULL;
    }
    
    engine->text_buf = C2D_TextBufNew(4096);
    
    // Store app callbacks
    engine->app = *app;
    engine->user_data = user_data;
    engine->running = false;
    engine->delta_time = 0.016f;  // ~60 FPS
    
    return engine;
}

void engine_destroy(Engine* engine) {
    if (!engine) return;
    
    C2D_TextBufDelete(engine->text_buf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
    
    free(engine);
}

void engine_run(Engine* engine) {
    if (!engine) return;
    
    // Call game init
    if (!engine->app.init(engine->user_data)) {
        printf("Game initialization failed!\n");
        return;
    }
    
    engine->running = true;
    
    // Main loop
    while (aptMainLoop() && engine->running) {
        // Input
        hidScanInput();
        u32 keys_down = hidKeysDown();
        u32 keys_held = hidKeysHeld();
        u32 keys_up = hidKeysUp();
        touchPosition touch;
        hidTouchRead(&touch);
        
        input_update(keys_down, keys_held, keys_up, touch);
        
        if (keys_down & KEY_START) {
            engine->running = false;
            break;
        }
        
        // Game update
        engine->app.update(engine->user_data);
        
        // Render frame
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        
        // Top screen
        C2D_TargetClear(engine->top, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(engine->top);
        
        // Bottom screen
        C2D_TargetClear(engine->bottom, C2D_Color32(0, 0, 0, 255));
        C2D_SceneBegin(engine->bottom);
        
        // Game render
        engine->app.render(engine->user_data);
        
        C3D_FrameEnd(0);
    }
    
    // Call game shutdown
    engine->app.shutdown(engine->user_data);
}

C3D_RenderTarget* engine_get_top_target(Engine* engine) {
    return engine ? engine->top : NULL;
}

C3D_RenderTarget* engine_get_bottom_target(Engine* engine) {
    return engine ? engine->bottom : NULL;
}

float engine_get_delta_time(Engine* engine) {
    return engine ? engine->delta_time : 0.0f;
}

bool engine_should_exit(Engine* engine) {
    return engine ? !engine->running : true;
}

void engine_request_exit(Engine* engine) {
    if (engine) engine->running = false;
}
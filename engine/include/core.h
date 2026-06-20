#pragma once
#include <citro2d.h>
#include <citro3d.h>
#include <stdbool.h>

typedef struct {
    // Called once at startup
    bool (*init)(void* user_data);
    
    // Called every frame
    void (*update)(void* user_data);
    void (*render)(void* user_data);
    
    // Called at shutdown
    void (*shutdown)(void* user_data);
} GameCallbacks;

typedef struct {
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
    C2D_TextBuf text_buf;
    
    GameCallbacks app;
    void* user_data;
    
    bool running;
    float delta_time;
} Engine;

// Engine lifecycle
Engine* engine_create(const GameCallbacks* app, void* user_data);
void engine_destroy(Engine* engine);

// Run the main loop
void engine_run(Engine* engine);

// Query engine state (for game to use)
C3D_RenderTarget* engine_get_top_target(Engine* engine);
C3D_RenderTarget* engine_get_bottom_target(Engine* engine);
float engine_get_delta_time(Engine* engine);
bool engine_should_exit(Engine* engine);
void engine_request_exit(Engine* engine);
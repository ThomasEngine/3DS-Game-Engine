#include "game_app.h"
#include "core.h"

int main() {
    // Create game
    GameApp game;
    
    // Create engine with game callbacks
    GameCallbacks app_callbacks = {
        .init = GameApp::s_init,
        .update = GameApp::s_update,
        .render = GameApp::s_render,
        .shutdown = GameApp::s_shutdown
    };
    
    Engine* engine = engine_create(&app_callbacks, &game);
    if (!engine) return 1;
    
    // Run!
    engine_run(engine);
    
    engine_destroy(engine);
    return 0;
}
#include "game_app.h"
#include "level.h"
#include "input.h"
#include "core.h"

GameApp::GameApp() : assets(nullptr), level(nullptr) {}

GameApp::~GameApp() {}

bool GameApp::init() {
    // Load graphics
    assets = graphics_load_assets();
    if (!assets) {
        printf("Failed to load graphics\n");
        return false;
    }
    
    // Create ECS world
    //world = ecs_create();
    //if (!world) {
    //    printf("Failed to create ECS world\n");
    //    return false;
    //}
    
    // Initialize game systems
    // Things like player and level.
    return true;
}

void GameApp::update() {
    // Handle input
    
    // If press start shutdown application
}

void GameApp::render() {
    printf("Siem is dik\n");
}

void GameApp::shutdown() {
    //ecs_destroy(world);
    graphics_free_assets(assets);
}

// C callbacks
bool GameApp::s_init(void* user_data) {
    return static_cast<GameApp*>(user_data)->init();
}

void GameApp::s_update(void* user_data) {
    static_cast<GameApp*>(user_data)->update();
}

void GameApp::s_render(void* user_data) {
    static_cast<GameApp*>(user_data)->render();
}

void GameApp::s_shutdown(void* user_data) {
    static_cast<GameApp*>(user_data)->shutdown();
}
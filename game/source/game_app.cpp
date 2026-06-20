#include "game_app.h"
#include "level.h"
#include "input.h"

GameApp::GameApp() : assets(nullptr), level(nullptr) {}

GameApp::~GameApp() {}

bool GameApp::init() {
    // Load graphics
    //assets = graphics_load_assets();
    /*if (!assets) {
        printf("Failed to load graphics\n");
        return false;
    } */
    
    // Initialize game systems
    // Things like player and level.
    return true;
}

void GameApp::update() {
    // Handle input
}

void GameApp::render() {
}

void GameApp::shutdown() {
    graphics_free_assets(assets);
    assets = nullptr;
}
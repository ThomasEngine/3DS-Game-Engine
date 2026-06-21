#include "game_app.h"

#include "ecs.h"
#include "level.h"
#include "input.h"
#include "systems/animation_system.h"
#include "systems/physics_system.h"
#include "systems/render_system.h"

GameApp::GameApp() : assets(nullptr), level(nullptr), world(nullptr) {}

GameApp::~GameApp() {}

Entity player = 0;

bool GameApp::init(ECSWorld *world, Renderer &renderer) {
    this->world = world;
    if (!world) return false;


    // Load graphics assets
    assets = graphics_load_assets();
    if (!assets) {
        return false;
    }


    player = world->createEntity();
    world->addComponent(player, COMP_POSITION | COMP_SPRITE);
    world->sprite[player].sprite.init(assets->sprites, 1);
    world->position[player] = {1.0f, 1.0f};

    // Initialize game systems
    // Things like player and level.
    return true;
}

void GameApp::update(float deltaTime) {
    update_physics_system(*world, deltaTime);
    update_animation_system(*world, deltaTime);
}

void GameApp::render(Renderer& renderer) {
    renderer.beginTopScreen();
    render_system_draw(*world, renderer, RenderLayer::LAYER_TOP);


    renderer.beginBottomScreen();
    render_system_draw(*world, renderer, RenderLayer::LAYER_BOTTOM);
}

void GameApp::shutdown() {
    graphics_free_assets(assets);
    assets = nullptr;
}
#include "game_app.h"

#include "ecs.h"
#include "level.h"
#include "input.h"
#include "systems/animation_system.h"
#include "systems/physics_system.h"
#include "systems/render_system.h"
#include "systems/collision_system.h"
#include "graphics.h"
#include "level_renderer.h"



GameApp::GameApp() : assets(nullptr), level(nullptr), world(nullptr) {}

GameApp::~GameApp() {}


bool GameApp::init(ECSWorld *world, Renderer &renderer) {
    this->world = world;
    if (!world) return false;

    // Load graphics assets
    assets = graphics_load_assets();
    if (!assets) {
        return false;
    }

    // Initialize game systems
    player.init(*world, assets, 1.f, 1.f);

    level = new Level();
    if (level) {
        level->loadDefault();
    }

    return true;
}
void GameApp::update(float deltaTime) {
    player.handleInput(*world);

    collision_system_update(
        *world,
        level->getGrid(),
        [&](int x, int y) {
            return level->isWalkable(x, y);
        },
        [&](CollisionEvent event, ECSWorld& world) {
            onCollision(event, world);

        },
        deltaTime
    );

    update_physics_system(*world, deltaTime);
    update_animation_system(*world, deltaTime);
}

void GameApp::onCollision(CollisionEvent event, ECSWorld& world) {
    Entity playerEntity = player.getEntity();
    Entity other = INVALID_ENTITY;

    if (event.a == playerEntity) other = event.b;
    if (event.b == playerEntity) other = event.a;
    if (other == INVALID_ENTITY) return;
}

void GameApp::render(Renderer& renderer) {
    renderer.beginTopScreen();
        draw_level(
        level->getWidth(),
        level->getHeight(),
        [&](int x, int y) {
            int index = level->getTileSprite(x, y);
            return C2D_SpriteSheetGetImage(assets->tiles, index);
        }
    );

    render_system_draw(*world, renderer, RenderLayer::LAYER_TOP);

    renderer.beginBottomScreen();
    render_system_draw(*world, renderer, RenderLayer::LAYER_BOTTOM);
}

void GameApp::shutdown() {
    graphics_free_assets(assets);
    assets = nullptr;
    delete level;
    level = nullptr;
}
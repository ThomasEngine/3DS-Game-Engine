#include "game_scene.h"

#include "level.h"
#include "input.h"
#include "systems/animation_system.h"
#include "systems/physics_system.h"
#include "systems/render_system.h"
#include "systems/collision_system.h"
#include "game_assets.h"
#include <algorithm>
#include "scene_manager.h"
#include "tiled_render.h"

void GameScene::enter() {
    assets = graphics_load_assets();

    player.init(world, assets, 1.f, 1.f, manager->getSettings());

    level = new Level();
    level->load("romfs:/levels/1vs1_level.tmj");
}

void GameScene::exit() {
    graphics_free_assets(assets);
    assets = nullptr;
    delete level;
    level = nullptr;
}

void GameScene::update(float deltaTime) {
    EngineSettings& settings = manager->getSettings();
    player.handleInput(world);

    systems::collision_update(
        world,
        [&](int x, int y) {
            return level->isWalkable(x, y);
        },
        [&](CollisionEvent event, ECSWorld& w) {
            onCollision(event, w);
        },
        deltaTime
    );

    systems::update_velocity(world, deltaTime, settings);
    systems::update_animation(world, deltaTime);
}

void GameScene::onCollision(CollisionEvent event, ECSWorld& world) {
    Entity playerEntity = player.getEntity();
    Entity other = INVALID_ENTITY;

    if (event.a == playerEntity) other = event.b;
    if (event.b == playerEntity) other = event.a;
    if (other == INVALID_ENTITY) return;
}

static float printX;
static float printY;

void GameScene::renderTop(Renderer& renderer) {
    world.sprite[player.getEntity()].layer = LAYER_TOP;
    // update camera to follow player
    Camera& cam = renderer.getCamera();
    const PositionComponent& position = world.position[player.getEntity()];
    // printX  = cam.x = position.x - SCREEN_TILES_W / 2.0f;
    // printY = cam.y = position.y - SCREEN_TILES_H / 2.0f;

    printX  = cam.x = position.x - 12.5 / 2.0f + 0.5;
    printY = cam.y = position.y - 7.5 / 2.0f + 0.5;

    cam.x = std::clamp(cam.x, 0.0f, (float)level->getWidth()  - 12.5f);
    cam.y = std::clamp(cam.y, 0.0f, (float)level->getHeight() - 7.5f);

    // draw_level(
    //     level->getWidth(),
    //     level->getHeight(),
    //     renderer.getCamera(),
    //     [&](int x, int y) {
    //         int index = level->getTileSprite(x, y);
    //         return C2D_SpriteSheetGetImage(assets->tiles, index);
    //     },
    //     manager->getSettings()
    // );

    draw_tiled_map(level->getMap(), cam, assets->tiles, manager->getSettings());

    systems::render_draw(world, renderer, RenderLayer::LAYER_TOP, manager->getSettings());
}

#include <string>
void GameScene::renderBottom(Renderer& renderer) {
    systems::render_draw(world, renderer, RenderLayer::LAYER_BOTTOM, manager->getSettings());

    // DEBUG
    const PositionComponent& position = world.position[player.getEntity()];


    char buf[64];
    snprintf(buf, sizeof(buf), "Cam X: %.2f Y: %.2f \n Player X %.2f %.2f", printX, printY, position.x, position.y);
    renderer.drawText(buf, 0, 0, C2D_Color32(255, 255, 255, 255), 1);
}
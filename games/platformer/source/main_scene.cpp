#include "main_scene.h"

#include <algorithm>

#include "animation_system.h"
#include "level.h"
#include  "tiled_render.h"
#include "background.h"
#include "cmath"
#include "collision_system.h"
#include "game_assets.h"
#include "physics_system.h"
#include "render_system.h"

void MainScene::enter() {
    const EngineSettings& s = manager->getSettings();

    assets = graphics_load_assets();

    level = new Level();
    level->load("romfs:/levels/prototype-level.tmj");

    Vec2 playerStartingPosPx = level->getPlayerStartPos();
    Vec2 playerStartPos;
    playerStartPos.x = playerStartingPosPx.x / s.tileSizePx;
    playerStartPos.y = playerStartingPosPx.y / s.tileSizePx;


    // player.init(world, assets, playerStartPos, s);
    player.init(world, assets, Vec2(2,2), s);
}

void MainScene::exit() {
}

void MainScene::update(float dt) {
    const EngineSettings& s = manager->getSettings();
    player.handleInput(world);

   systems::collision_update(
       world,
       [&](int x, int y) {
           return level->isWalkable(x, y);
       }, [&](CollisionEvent event, ECSWorld& w) {
           onCollision(event, w);
       }, dt);

    systems::update_gravity(world, dt, s);
    systems::update_velocity(world, dt, s);
    systems::update_animation(world, dt);
}


void MainScene::renderBottom(Renderer& renderer) {
}

void MainScene::onCollision(CollisionEvent event, ECSWorld &world) {
    // What happends when 2 enitty collides
}

void MainScene::renderTop(Renderer& renderer) {
    const EngineSettings& s = manager->getSettings();
    Camera& cam = renderer.getCamera();

    cam.x = world.position[player.getEntity()].x / 2.0 + 0.5;
    cam.y = world.position[player.getEntity()].y / 2.0 + 0.5;

    cam.x = std::clamp(cam.x, 0.0f, (float)level->getWidth()  - 12.5f);
    cam.y = std::clamp(cam.y, 0.0f, (float)level->getHeight() - 7.5f);

    draw_parallax_backgrounds(assets->backgrounds, cam, s);
    draw_tiled_map(level->getMap(), cam, assets->tiles, s);
    systems::render_draw(world, renderer, RenderLayer::LAYER_TOP, s);
}



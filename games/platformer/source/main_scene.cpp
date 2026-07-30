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
    // Get settings refference
    const EngineSettings& s = manager->getSettings();

    // Load graphics
    assets = graphics_load_assets();

    // Init level
    level = new Level();
    level->load("romfs:/levels/prototype-level.tmj");

    // Set player to position from tiled
    vec2 playerStartingPosPx = level->getPlayerStartPos();
    vec2 playerStartPos;
    playerStartPos.x = playerStartingPosPx.x / s.tileSizePx;
    playerStartPos.y = playerStartingPosPx.y / s.tileSizePx;

    // init player
    player.init(world, assets, vec2(2,2), s);

    // Set camera settings for this scene
    Camera& cam = renderer->getCamera();
    cam.settings.deadZoneW = 0.5f;
    cam.settings.deadZoneH = 1.0f;
    cam.settings.dampingX = 6.0f;
    cam.settings.dampingY = 8.0f;
    cam.settings.offsetY = 1.5f;
    cam.settings.lookAheadDistance = 1.0f;

    // level bounds
    cam.minX = 0.0f;
    cam.maxX = level->getWidth() - 12.5f;
    cam.minY = 0.0f;
    cam.maxY = level->getHeight() - 7.5f;

    // cam target
    cam.target = player.getEntity();
}

void MainScene::exit() {
}

void MainScene::update(float dt) {
    const EngineSettings& s = manager->getSettings();
    player.handleInput(world, dt);
    player.updateAnimation(world);

   systems::collision_update(
       world,
       [&](int x, int y) {
           return level->isWalkable(x, y);
       }, [&](CollisionEvent event, ECSWorld& w) {
           onCollision(event, w);
       }, dt,
       s);

    systems::update_gravity(world, dt, s);
    systems::update_velocity(world, dt, s);
    systems::update_animation(world, dt);

    int facing = player.isFacingLeft(world) ? -1 : 1;

    camera_update(world, renderer->getCamera(), facing, dt, s);
}



void MainScene::renderBottom() {
}

void MainScene::onCollision(CollisionEvent event, ECSWorld &world) {
    // What happends when 2 enitty collides
}

void MainScene::renderTop() {
    const EngineSettings& s = manager->getSettings();
    Camera& cam = renderer->getCamera();

    draw_parallax_backgrounds(assets->backgrounds, cam, s);
    draw_tiled_map(level->getMap(), cam, assets->tiles, s);
    systems::render_draw(world, *renderer, RenderLayer::LAYER_TOP, s);
}



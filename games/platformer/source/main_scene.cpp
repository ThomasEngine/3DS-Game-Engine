#include "main_scene.h"

#include <algorithm>

#include "level.h"
#include  "tiled_render.h"
#include "background.h"
#include "cmath"
#include "game_assets.h"
#include "input.h"

void MainScene::enter() {
    level = new Level();

    level->load("romfs:/levels/prototype-level.tmj");

    assets = graphics_load_assets();
}

void MainScene::exit() {
}

static float posX;
void MainScene::update(float dt) {
    if (input_held(KEY_CPAD_LEFT)) {
        posX -= 10 * dt;
    }
    if (input_held(KEY_CPAD_RIGHT)) {
        posX += 10 * dt;
    }

    camera.x = std::clamp(posX, 0.0f, (float)level->getWidth() - 12.5f);
}


void MainScene::renderBottom(Renderer& renderer) {
}

void MainScene::renderTop(Renderer& renderer) {
    const EngineSettings& s = manager->getSettings();

    draw_parallax_backgrounds(assets->backgrounds, camera, s);
    draw_tiled_map(level->getMap(), camera, assets->tiles, s);
}



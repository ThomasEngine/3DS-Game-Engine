#include "player.h"
#include "animations.h"
#include "input.h"

void Player::init(ECSWorld& world, GraphicsAssets* assets, float startX, float startY, const EngineSettings& settings) {
    Entity player = world.createEntity();
    entity = player;



    world.addComponent(player, COMP_POSITION | COMP_SPRITE | COMP_VELOCITY);
    world.sprite[player].sprite.init(assets->sprites, 1, settings);
    world.position[player] = {startX, startY};
    world.velocity[player] = {0.0f, 0.0f};

    speed = 5.0f;
}

void Player::destroy(ECSWorld& world) {
    world.destroyEntity(entity);
}
void Player::handleInput(ECSWorld& world) {
    world.velocity[entity].dx = 0.0f;
    world.velocity[entity].dy = 0.0f;

    bool moving = false;
    Sprite& sprite = world.sprite[entity].sprite;

    if (input_held(KEY_CPAD_RIGHT) || input_held(KEY_RIGHT)) {
        world.velocity[entity].dx =  speed;
        sprite.playAnimation(&PlayerAnims::WALK_RIGHT);
        moving = true;
    }
    if (input_held(KEY_CPAD_LEFT) || input_held(KEY_LEFT)) {
        world.velocity[entity].dx = -speed;
        sprite.playAnimation(&PlayerAnims::WALK_LEFT);
        moving = true;
    }
    if (input_held(KEY_CPAD_UP) || input_held(KEY_UP)) {
        world.velocity[entity].dy = -speed;
        sprite.playAnimation(&PlayerAnims::WALK_UP);
        moving = true;
    }
    if (input_held(KEY_CPAD_DOWN) || input_held(KEY_DOWN)) {
        world.velocity[entity].dy =  speed;
        sprite.playAnimation(&PlayerAnims::WALK_DOWN);
        moving = true;
    }

    if (!moving) sprite.stopAnimation();
}

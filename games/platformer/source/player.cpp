#include "../include/player.h"

#include "../include/animations.h"
#include "input.h"

void Player::init(ECSWorld &world, GraphicsAssets *assets, Vec2 startPos, const EngineSettings &settings) {
    Entity player = world.createEntity();
    entity = player;

    world.addComponent(player, COMP_POSITION | COMP_SPRITE | COMP_VELOCITY | COMP_GRAVITY);
    world.sprite[player].sprite.init(assets->player, 1, settings);
    world.position[player] = {startPos.x, startPos.y};
    world.velocity[player] = {0.0f, 0.0f};

    speed = 5.0f;
}

void Player::destroy(ECSWorld &world) {
    world.destroyEntity(entity);
}

void Player::handleInput(ECSWorld &world) {
    world.velocity[entity].dx = 0.0f;
    world.velocity[entity].dy = 0.0f;

    bool moving = false;
    Sprite& sprite = world.sprite[entity].sprite;

    // move left
    if (input_held(KEY_CPAD_RIGHT) || input_held(KEY_RIGHT)) {
        world.velocity[entity].dx = speed;
        sprite.playAnimation(&PlayerAnims::RUN);
        moving = true;
    }

    // move right
    if (input_held(KEY_CPAD_LEFT) || input_held(KEY_LEFT)) {
        world.velocity[entity].dx = -speed;
        sprite.playAnimation(&PlayerAnims::RUN);
        sprite.setFlipX(true);
        moving = true;
    }

    // jump
    if (input_held(KEY_B)) {
        if (world.gravity[entity].grounded) {
            world.velocity[entity].dy = -world.gravity[entity].jumpForce;
        }
    }

    if (input_released(KEY_B) && world.velocity[entity].dy < 0) {
        world.velocity[entity].dy *= 0.4f;
    }

    if (!moving) sprite.playAnimation(&PlayerAnims::IDLE);
}






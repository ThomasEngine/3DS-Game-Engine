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

    input::bind(Action::ACT_LEFT, KEY_CPAD_LEFT | KEY_LEFT);
    input::bind(Action::ACT_RIGHT, KEY_CPAD_RIGHT | KEY_RIGHT);
    input::bind(Action::ACT_UP, KEY_CPAD_UP | KEY_UP);
    input::bind(Action::ACT_DOWN, KEY_CPAD_DOWN | KEY_DOWN);
}

void Player::destroy(ECSWorld& world) {
    world.destroyEntity(entity);
}
void Player::handleInput(ECSWorld& world) {
    world.velocity[entity].dx = 0.0f;
    world.velocity[entity].dy = 0.0f;

    bool moving = false;
    Sprite& sprite = world.sprite[entity].sprite;

    if (input::held(ACT_RIGHT)) {
        world.velocity[entity].dx =  speed;
        sprite.playAnimation(&PlayerAnims::WALK_RIGHT);
        sprite.setFlipX(false);
        moving = true;
    }
    if (input::held(ACT_LEFT)) {
        world.velocity[entity].dx = -speed;
        sprite.playAnimation(&PlayerAnims::WALK_LEFT);
        sprite.setFlipX(true);
        moving = true;
    }
    if (input::held(ACT_UP)) {
        world.velocity[entity].dy = -speed;
        sprite.playAnimation(&PlayerAnims::WALK_UP);
        moving = true;
    }
    if (input::held(ACT_DOWN)) {
        world.velocity[entity].dy =  speed;
        sprite.playAnimation(&PlayerAnims::WALK_DOWN);
        moving = true;
    }

    if (!moving) sprite.stopAnimation();
}

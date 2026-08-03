#include "player.h"
#include "animations.h"
#include "input.h"

void Player::init(ECSWorld& world, GraphicsAssets* assets, float startX, float startY, const EngineSettings& settings) {
    Entity player = world.createEntity();
    entity = player;

    world.addComponent(player, COMP_POSITION | COMP_SPRITE | COMP_VELOCITY);
    world.sprite[player].sprite.init(assets->sprites, 1, settings);
    world.position[player].pos = {startX, startY};
    world.velocity[player].dir = {0.0f, 0.0f};

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
    vec2& velocity = world.velocity[entity].dir;
    velocity.x = 0.0f;
    velocity.y = 0.0f;

    bool moving = false;
    Sprite& sprite = world.sprite[entity].sprite;

    if (input::held(ACT_RIGHT)) {
        velocity.x =  speed;
        sprite.playAnimation(&PlayerAnims::WALK_RIGHT);
        moving = true;
    }
    if (input::held(ACT_LEFT)) {
        velocity.x = -speed;
        sprite.playAnimation(&PlayerAnims::WALK_LEFT);
        moving = true;
    }
    if (input::held(ACT_UP)) {
        velocity.y = -speed;
        sprite.playAnimation(&PlayerAnims::WALK_UP);
        moving = true;
    }
    if (input::held(ACT_DOWN)) {
        velocity.y =  speed;
        sprite.playAnimation(&PlayerAnims::WALK_DOWN);
        moving = true;
    }

    if (!moving) sprite.stopAnimation();
}

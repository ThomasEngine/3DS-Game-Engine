#include "player.h"

#include "animations.h"
#include "input.h"

void Player::init(ECSWorld &world, GraphicsAssets *assets, vec2 startPos, const EngineSettings &settings) {
    Entity player = world.createEntity();
    entity = player;

    world.addComponent(player, COMP_POSITION | COMP_SPRITE | COMP_VELOCITY | COMP_GRAVITY);
    world.sprite[player].sprite.init(assets->player, 1, settings);
    world.position[player] = {{startPos.x, startPos.y}, 0.35, 0.45};
    world.velocity[player].dir = {0.0f, 0.0f};

    speed = 6.0f;

    // init input actions
    input::bind(Action::ACT_JUMP, KEY_B);
    input::bind(Action::ACT_LEFT, KEY_CPAD_LEFT | KEY_LEFT);
    input::bind(Action::ACT_RIGHT, KEY_CPAD_RIGHT | KEY_RIGHT);
}

void Player::destroy(ECSWorld &world) {
    world.destroyEntity(entity);
}

void Player::handleInput(ECSWorld &world, float dt) {
    int& grounded = world.gravity[entity].grounded;

    vec2& vel = world.velocity[entity].dir;

    if (was_grounded && !grounded) {
        coyote_timer = coyote_duration;
    }

    const float accel = 40.f;

    // Moving left and right
    if (input::held(ACT_RIGHT)) {
        vel.x += accel * dt;
        if (vel.x > speed) vel.x = speed;
    }
    if (input::held(ACT_LEFT)) {
        vel.x -= accel * dt;
        if (vel.x < -speed) vel.x = -speed;
    }


    // Jumping
    // Coyote timer
    if (!grounded) {
        coyote_timer -= dt;
    }

    if (input::action_buffered(ACT_JUMP) && (grounded || coyote_timer > 0.0f) ) {
        vel.y = -world.gravity[entity].jumpForce;
        grounded = 0;
        coyote_timer = 0.0f;

        input::action_buffer_use(ACT_JUMP);
    }

    if (input::released(ACT_JUMP) && vel.y < 0) {
        vel.y *= 0.4f;
    }


    if (input::pressed(KEY_X)) {
        // reset player
        world.position[entity].pos.x = 2;
        world.position[entity].pos.y = 2;

    }

    was_grounded = grounded;
}

void Player::updateAnimation(ECSWorld& world) {
    Sprite& sprite = world.sprite[entity].sprite;
    bool grounded = world.gravity[entity].grounded != 0;

    vec2& velocity = world.velocity[entity].dir;
    float& dx = velocity.x;
    float& dy = velocity.y;
    if (dx < 0.5 && dx > -0.5) {
        dx = 0;
    }

    if (!grounded) {
        if (dy < 0) sprite.playAnimation(&PlayerAnims::JUMP);
        else sprite.playAnimation(&PlayerAnims::FALL);
    } else if (dx != 0.0f) {
        sprite.playAnimation(&PlayerAnims::RUN);
    } else {
        sprite.playAnimation(&PlayerAnims::IDLE);
    }

    if (dx > 0) { sprite.setFlipX(false); facing_left = false; }
    else if (dx < 0) { sprite.setFlipX(true); facing_left = true;}
}

bool Player::isFacingLeft(ECSWorld &world) {
    return facing_left;
}
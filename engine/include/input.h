#pragma once
#include <3ds.h>

enum Action {
    ACT_JUMP,
    ACT_ATTACK,
    ACT_LEFT,
    ACT_RIGHT,
    ACT_UP,
    ACT_DOWN,
    ACT_INTERACT,
    ACT_PAUSE,
    ACT_COUNT
};

void input_update(u32 keys_down, u32 keys_held, u32 keys_up, touchPosition touch);

bool input_pressed(u32 key); // This frame
bool input_held(u32 key); // Any frame
bool input_released(u32 key); // This frame
touchPosition input_get_touch();

void input_update_actions(float dt);

bool input_action_buffered(Action a);
void input_buffer_use(Action a);

void input_bind(Action a, u32 keys);
void input_unbind(Action a);
void input_clear_all();

bool input_action_pressed(Action a);
bool input_action_held(Action a);
bool input_action_released(Action a);

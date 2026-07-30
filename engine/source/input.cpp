#include "input.h"

static u32 actionBindings[ACT_COUNT] = {0};

static float actionBuffer[ACT_COUNT] = {0};
static constexpr float BUFFER_WINDOW = 0.25f; // in seconds

static u32 current_keys_down = 0;
static u32 current_keys_held = 0;
static u32 current_keys_up = 0;
static touchPosition current_touch = {0};

void input_update(u32 keys_down, u32 keys_held, u32 keys_up, touchPosition touch) {
    current_keys_down = keys_down;
    current_keys_held = keys_held;
    current_keys_up = keys_up;
    current_touch = touch;
}

bool input_pressed(u32 key) {
    return (current_keys_down & key) != 0;
}

bool input_held(u32 key) {
    return (current_keys_held & key) != 0;
}

bool input_released(u32 key) {
    return (current_keys_up & key) != 0;
}

touchPosition input_get_touch() {
    return current_touch;
}

void input_update_actions(float dt) {
    for (int a = 0; a < ACT_COUNT; a++) {
        if (input_action_pressed((Action)a)) {
            actionBuffer[a] = BUFFER_WINDOW;
        } else {
            actionBuffer[(Action)a] -= dt;
            if (actionBuffer[Action(a)] < 0) actionBuffer[Action(a)] = 0;
        }
    }
}

bool input_action_buffered(Action a) {
    return actionBuffer[a] > 0.0f;
}

void input_buffer_use(Action a) {
    actionBuffer[a] = 0.0f;
}


void input_bind(Action a, u32 keys) {
    if (a < 0 || a >= ACT_COUNT) return;

    actionBindings[a] |= keys; //
}

void input_unbind(Action a) {
    if (a < 0 || a >= ACT_COUNT) return;

    actionBindings[a] = 0;
}

void input_clear_all() {
    for (u32 i = 0; i < ACT_COUNT; i++) actionBindings[i] = 0;
}

bool input_action_pressed(Action a) {
    if (a < 0 || a >= ACT_COUNT) return false;
    return (current_keys_down & actionBindings[a]) != 0;
}

bool input_action_held(Action a) {
    return (current_keys_held & actionBindings[a]) != 0;
}

bool input_action_released(Action a) {
    if (a < 0 || a >= ACT_COUNT) return false;
    return (current_keys_up & actionBindings[a]) != 0;
}

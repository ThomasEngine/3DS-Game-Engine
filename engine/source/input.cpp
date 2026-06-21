#include "input.h"

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

touchPosition input_get_touch(void) {
    return current_touch;
}

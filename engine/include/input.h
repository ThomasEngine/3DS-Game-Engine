#pragma once
#include <3ds.h>

void input_update(u32 keys_down, u32 keys_held, u32 keys_up, touchPosition touch);

bool input_pressed(u32 key);   // This frame
bool input_held(u32 key);      // Any frame
bool input_released(u32 key);  // This frame
touchPosition input_get_touch(void);

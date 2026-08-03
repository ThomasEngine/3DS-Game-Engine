#include "input.h"

namespace input {
    static u32 actionBindings[ACT_COUNT] = {0};

    static float actionBuffer[ACT_COUNT] = {0};
    static constexpr float BUFFER_WINDOW = 0.25f; // in seconds

    static u32 current_keys_down = 0;
    static u32 current_keys_held = 0;
    static u32 current_keys_up = 0;
    static touchPosition current_touch = {0};

    void update(u32 keys_down, u32 keys_held, u32 keys_up, touchPosition touch, float dt) {
        // Update regular input
        current_keys_down = keys_down;
        current_keys_held = keys_held;
        current_keys_up = keys_up;
        current_touch = touch;

        // Update actions
        for (int a = 0; a < ACT_COUNT; a++) {
            if (pressed((Action)a)) {
                actionBuffer[a] = BUFFER_WINDOW;
            } else {
                actionBuffer[(Action)a] -= dt;
                if (actionBuffer[Action(a)] < 0) actionBuffer[Action(a)] = 0;
            }
        }
    }

    // Normal input
    bool pressed(u32 key) {
        return (current_keys_down & key) != 0;
    }

    bool held(u32 key) {
        return (current_keys_held & key) != 0;
    }

    bool released(u32 key) {
        return (current_keys_up & key) != 0;
    }

    touchPosition get_touch() {
        return current_touch;
    }

    // Input action
    bool pressed(Action a) {
        if (a < 0 || a >= ACT_COUNT) return false;
        return (current_keys_down & actionBindings[a]) != 0;
    }

    bool held(Action a) {
        return (current_keys_held & actionBindings[a]) != 0;
    }

    bool released(Action a) {
        if (a < 0 || a >= ACT_COUNT) return false;
        return (current_keys_up & actionBindings[a]) != 0;
    }

    void bind(Action a, u32 keys) {
        if (a < 0 || a >= ACT_COUNT) return;
        actionBindings[a] |= keys;
    }

    void unbind(Action a, u32 keys) {
        if (a < 0 || a >= ACT_COUNT) return;
        actionBindings[a] = 0;
    }

    bool action_buffered(Action a) {
        return actionBuffer[a] > 0.0f;
    }

    void action_buffer_use(Action a) {
        actionBuffer[a] = 0.0f;
    }
}
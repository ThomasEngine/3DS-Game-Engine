#pragma once

// Default engine settings

struct EngineSettings {
    // physics
    float gravity        = 30.8f;
    float maxFallSpeed   = 10.0f;
    bool  useFriction    = false;
    float friction       = 0.9f;

    // rendering
    int   tileSizePx     = 16;
    float tileScale      = 2.0f;
    float entityScale    = 2.0f;

    // collision
    bool  useTileSnapping = true;
    float snapThreshold   = 0.3f;
};
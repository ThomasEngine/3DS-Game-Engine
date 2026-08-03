#pragma once
#include "ecs.h"

struct CameraSettings {
    float deadZoneW = 1.0f;
    float deadZoneH = 1.0f;

    float lookAheadDistance = 2.0f;
    float lookAheadSmoothing = 3.0f; // higher is snappier

    // Damping higher is snappier
    float dampingX = 5.0f;
    float dampingY = 8.0f;

    // player offset from screen center
    float offsetX = 0.5; // 0.5 for sprite centering (real center)
    float offsetY = 1.5; // player slightly below center

    // vertical framing
    float softDropThreshold = 5.0f; // don't track falls below this dy velocity
    float fallCatchDelay = 0.3f; // seconds before tracking fast falls
};

struct Camera {
    // state, changes every frame
    float x, y = 0.0f;
    float lookAheadX = 0.0f, lookAheadY = 0.0f;
    float shakeIntensity = 0.0f;
    float shakeX = 0.0f, shakeY = 0.0f;

    // Settings config
    CameraSettings settings;

    // bounds, level specific
    float minX, maxX, minY, maxY;

    int SCREEN_WIDTH = 400;
    int SCREEN_HEIGHT = 240;

    // target to track
    Entity target = INVALID_ENTITY;
};


void camera_update(ECSWorld &world, Camera &cam, int facing, float dt, const EngineSettings &settings);

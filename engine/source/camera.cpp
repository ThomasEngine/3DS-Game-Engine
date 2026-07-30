#include "camera.h"

#include <algorithm>
#include <cmath>

void camera_update(ECSWorld &world, Camera &cam, int facing, float dt, const EngineSettings &settings) {
    const CameraSettings& s = cam.settings;

    Entity target = cam.target;
    if (!world.isValid(target)) return;
    if (!world.hasComponent(target, COMP_POSITION)) return;

    // screen dims
    float pxPerTile = settings.tileSizePx * settings.tileScale;
    float halfW = (400.0f / pxPerTile) / 2.0f;
    float halfH = (240.0f / pxPerTile) / 2.0f;

    const PositionComponent& pos = world.position[target];
    float desiredX = pos.x + s.offsetX - halfW;
    float desiredY = pos.y + s.offsetY - halfH;

    // check if target is moving
    bool moving = false;
    if (world.hasComponent(target, COMP_VELOCITY)) {
        const VelocityComponent& vel = world.velocity[target];
        float speedSq = vel.dx * vel.dx + vel.dy * vel.dy;
        moving = speedSq > (s.idleThreshold * s.idleThreshold);
    }

    // lookahead — only when actively moving
    float targetLookX = moving ? (facing * s.lookAheadDistance) : 0.0f;
    cam.lookAheadX += (targetLookX - cam.lookAheadX) * s.lookAheadSmoothing * dt;
    desiredX += cam.lookAheadX;

    // dead zone — only when idle
    float dx = desiredX - cam.x;
    float dy = desiredY - cam.y;

    if (!moving) {
        // apply dead zone
        if (std::abs(dx) < s.deadZoneW) dx = 0.0f;
        else dx -= (dx > 0 ? s.deadZoneW : -s.deadZoneW);
        if (std::abs(dy) < s.deadZoneH) dy = 0.0f;
        else dy -= (dy > 0 ? s.deadZoneH : -s.deadZoneH);
    }
    // when moving, don't apply dead zone → camera targets player directly

    // damping (with snap-threshold)
    const float SNAP = 0.005f;
    if (std::abs(dx) < SNAP) cam.x += dx;
    else cam.x += dx * s.dampingX * dt;
    if (std::abs(dy) < SNAP) cam.y += dy;
    else cam.y += dy * s.dampingY * dt;

    // clamp
    cam.x = std::clamp(cam.x, cam.minX, cam.maxX);
    cam.y = std::clamp(cam.y, cam.minY, cam.maxY);
    ;


    // TODO:: SHAKE
}

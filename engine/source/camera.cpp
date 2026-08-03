#include "camera.h"

#include <algorithm>
#include <cmath>

void camera_update(ECSWorld &world, Camera &cam, int facing, float dt, const EngineSettings &settings) {
    const CameraSettings &s = cam.settings;

    Entity target = cam.target;

    if (!world.isValid(target)) return;
    if (!world.hasComponent(target, COMP_POSITION)) return;


    vec2 targetPos = world.position[target].pos;

    float halfScreenW = 12.5 / 2.0f;
    float halfScreenH = 7.5 / 2.0f;

    float desiredX = targetPos.x + s.offsetX - halfScreenW;
    float desiredY = targetPos.y + s.offsetY - halfScreenH;

    // dead zone check, is the target outside the dead zone?
    float rawDx = desiredX - cam.x;

    bool outsideDeadZoneX = std::abs(rawDx) > s.deadZoneW;

    // apply lookahead
    float targetLookX = outsideDeadZoneX ? (facing * s.lookAheadDistance) : 0.0f;
    cam.lookAheadX += (targetLookX - cam.lookAheadX) * s.lookAheadSmoothing * dt;
    desiredX += cam.lookAheadX;

    // dead zone only moves camera if target is outside it
    float dx = desiredX - cam.x;
    float dy = desiredY - cam.y;

    if (std::abs(dx) < s.deadZoneW) dx = 0.0f;
    else dx -= (dx > 0 ? s.deadZoneW : -s.deadZoneW);
    if (std::abs(dy) < s.deadZoneH) dy = 0.0f;
    else dy -= (dy > 0 ? s.deadZoneH : -s.deadZoneH);

    // damping
    cam.x += dx * s.dampingX * dt;
    cam.y += dy * s.dampingY * dt;

    // clamp
    cam.x = std::clamp(cam.x, cam.minX, cam.maxX);
    cam.y = std::clamp(cam.y, cam.minY, cam.maxY);


    // TODO:: SHAKE
}

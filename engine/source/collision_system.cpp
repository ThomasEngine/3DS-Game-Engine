#include "systems/collision_system.h"

#include "ecs.h"
#include "spatial_grid.h"

#include "cmath"

#include "sprite.h" // only for debugging remove when not

namespace systems {
    // Collision helpers
    float toTile(float f) { return std::round(f); }
    float tileCenter(float f) { return std::floor(f) + 0.5f; }

    // Collision checker
    void collision_update(ECSWorld &world, WalkableCallback isWalkable, CollisionCallback onEntityCollision, float dt,
                          const EngineSettings &settings) {
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            if (!world.isValid(e)) continue;
            if (!world.hasComponent(e, COMP_POSITION | COMP_VELOCITY)) continue;

            PositionComponent& entityPosComp = world.position[e];
            vec2 &entityPos = entityPosComp.pos;

            vec2 &entityVel = world.velocity[e].dir;

            // predict next position
            float newX = entityPos.x + entityVel.x * dt;
            float newY = entityPos.y + entityVel.y * dt;

            const float HALFW = entityPosComp.halfWidth;
            const float HALFH = entityPosComp.halfHeight;

            const float INSET = 0.01f;


            // If WalkableCallback is there, check for collision
            if (isWalkable) {
                // x movement
                if (entityVel.x != 0.0f) {
                    float checkX = newX + (entityVel.x > 0 ? HALFW : -HALFW);


                    bool topBlocked = !isWalkable(toTile(checkX), toTile(entityPos.y - HALFH + INSET));
                    bool bottomBlocked = !isWalkable(toTile(checkX), toTile(entityPos.y + HALFH - INSET));

                    if (!settings.useTileSnapping) {
                        if (topBlocked || bottomBlocked) {
                            entityVel.x = 0.0f;
                        }
                    } else {
                        const float SNAP = 5;
                        if (topBlocked && bottomBlocked) {
                            entityVel.x = 0.0f;
                        } else if (topBlocked) {
                            entityVel.x = 0.0f;
                            if (isWalkable(toTile(entityPos.x), toTile(entityPos.y + HALFW + 0.1f))) {
                                float target = tileCenter(entityPos.y) + 1.0f;
                                float diff = target - entityPos.y;
                                float move = (diff > 0 ? SNAP : -SNAP);
                                if (std::abs(move * dt) > std::abs(diff)) {
                                    entityPos.y = target;
                                    entityVel.y = 0.0f;
                                } else {
                                    entityVel.y = move;
                                }
                            }
                        } else if (bottomBlocked) {
                            entityVel.x = 0.0f;
                            if (isWalkable(toTile(entityPos.x), toTile(entityPos.y - HALFW - 0.1f))) {
                                float target = tileCenter(entityPos.y) - 1.0f;
                                float diff = target - entityPos.y;
                                float move = (diff > 0 ? SNAP : -SNAP);
                                if (std::abs(move * dt) > std::abs(diff)) {
                                    entityPos.y = target;
                                    entityVel.y = 0.0f;
                                } else {
                                    entityVel.y = move;
                                }
                            }
                        }
                    }
                }
                // y movement
                if (entityVel.y != 0.0f) {
                    float checkY = newY + (entityVel.y > 0 ? HALFH : -HALFH);
                    bool leftBlocked = !isWalkable(toTile(entityPos.x - HALFW + INSET), toTile(checkY));
                    bool rightBlocked = !isWalkable(toTile(entityPos.x + HALFW - INSET), toTile(checkY));

                    if (!settings.useTileSnapping) {
                        if (leftBlocked || rightBlocked) {
                            entityVel.y = 0.0f;
                        }
                    } else {
                        const float SNAP = 5;
                        if (leftBlocked && rightBlocked) {
                            entityVel.y = 0.0f;
                        } else if (leftBlocked) {
                            entityVel.y = 0.0f;
                            if (isWalkable(toTile(entityPos.x + HALFH + 0.1f), toTile(entityPos.y))) {
                                float target = tileCenter(entityPos.x) + 1.0f;
                                float diff = target - entityPos.x;
                                float move = (diff > 0 ? SNAP : -SNAP);
                                if (std::abs(move * dt) > std::abs(diff)) {
                                    entityPos.x = target;
                                    entityVel.x = 0.0f;
                                } else {
                                    entityVel.x = move;
                                }
                            }
                        } else if (rightBlocked) {
                            entityVel.y = 0.0f;
                            if (isWalkable(toTile(entityPos.x - HALFH - 0.1f), toTile(entityPos.y))) {
                                float target = tileCenter(entityPos.x) - 1.0f;
                                float diff = target - entityPos.x;
                                float move = (diff > 0 ? SNAP : -SNAP);
                                if (std::abs(move * dt) > std::abs(diff)) {
                                    entityPos.x = target;
                                    entityVel.x = 0.0f;
                                } else {
                                    entityVel.x = move;
                                }
                            }
                        }
                    }
                }
                if (world.hasComponent(e, COMP_GRAVITY)) {
                    float feetY = entityPos.y + HALFH;

                    int leftX = (int) std::round(entityPos.x - HALFW);
                    int rightX = (int) std::round(entityPos.x + HALFW);
                    int ty = (int) std::round(feetY + 0.02f);

                    bool solidBelow = !isWalkable(leftX, ty) || !isWalkable(rightX, ty);
                    bool notMovingUp = entityVel.y >= 0.0f;

                    world.gravity[e].grounded = (solidBelow && notMovingUp) ? 1 : 0;
                }
            }
            // entity collision
            if (onEntityCollision) {
                for (Entity a = 0; a < MAX_ENTITIES; a++) {
                    if (!world.isValid(a)) continue;
                    if (!world.hasComponent(a, COMP_POSITION)) continue;

                    for (Entity b = a + 1; b < MAX_ENTITIES; b++) {
                        if (!world.isValid(b)) continue;
                        if (!world.hasComponent(b, COMP_POSITION)) continue;

                        // check if a and b are close enough to collide
                        vec2 diff = world.position[a].pos - world.position[b].pos;

                        if (diff.dot(diff) < 1.0f) {
                            onEntityCollision({a, b}, world);
                        }
                    }
                }
            }
        }
    }
}

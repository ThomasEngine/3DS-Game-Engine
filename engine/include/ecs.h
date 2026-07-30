#pragma once
#include <array>
#include <stdint.h>
#include "sprite.h"
#include "ai_state.h"

// Entity is an index
using Entity = uint32_t;
constexpr size_t MAX_ENTITIES = 128;
constexpr Entity INVALID_ENTITY = MAX_ENTITIES;

// Component flags
enum ComponentFlags : uint32_t {
    COMP_NONE = 0,
    COMP_POSITION = 1 << 0, // 10000000
    COMP_VELOCITY = 1 << 1, // 01000000
    COMP_SPRITE = 1 << 2, // 001000000
    COMP_HEALTH = 1 << 3,
    COMP_AI = 1 << 4,
    COMP_PATROL = 1 << 5,
    COMP_GRAVITY = 1 << 6
};

// Components
struct PositionComponent {
    float x = 0.0f;
    float y = 0.0f;
    float halfWidth = 0.45f;
    float halfHeight = 0.45f;
};

struct VelocityComponent {
    float dx = 0.0f;
    float dy = 0.0f;
};

struct GravityComponent {
    float weight = 1.0f;
    float jumpForce = 12.0f;
    int grounded = 0;
};

enum RenderLayer : uint32_t {
    LAYER_TOP = 0,
    LAYER_BOTTOM = 1,
};

struct SpriteComponent {
    Sprite sprite;
    RenderLayer layer = LAYER_TOP;
};

struct HealthComponent {
    int current = 0;
    int max = 0;
};

struct AIComponent {
    AIState state = AI_IDLE;
    AIState previousState = AI_IDLE;
    float speed = 1.0f;
    Entity target = INVALID_ENTITY;
    float detectionRange = 5.0f;
    float wanderTimer = 0.0f;
    float stunTimer = 0.0f;
};

struct PatrolComponent {
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = 0.0f;
    float y2 = 0.0f;
    int goingToB = (int) true; // direction of travel
};

// The world owns all entity and component data
class ECSWorld {
public:
    ECSWorld() {
        active.fill(false);
        componentMask.fill(COMP_NONE);
    }

    // Entity state
    std::array<bool, MAX_ENTITIES> active;
    std::array<uint32_t, MAX_ENTITIES> componentMask;

    // Component arrays
    std::array<PositionComponent, MAX_ENTITIES> position;
    std::array<VelocityComponent, MAX_ENTITIES> velocity;
    std::array<GravityComponent, MAX_ENTITIES> gravity;
    std::array<SpriteComponent, MAX_ENTITIES> sprite;
    std::array<HealthComponent, MAX_ENTITIES> health;
    std::array<AIComponent, MAX_ENTITIES> ai;
    std::array<PatrolComponent, MAX_ENTITIES> patrol;

    // Entity management
    Entity createEntity();

    void destroyEntity(Entity e);

    bool isValid(Entity e);

    bool hasComponent(Entity e, uint32_t flag) const;

    void addComponent(Entity e, uint32_t flag);

    void removeComponent(Entity e, uint32_t flag);
};

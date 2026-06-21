#include "ecs.h"

Entity ECSWorld::createEntity() {
    for (Entity i = 0; i < MAX_ENTITIES; i++) {
        if (!active[i]) {
            active[i] = true;
            componentMask[i] = COMP_NONE;
            return i;
        }
    }
    return INVALID_ENTITY;
}

void ECSWorld::destroyEntity(Entity e) {
    if (!isValid(e)) return;
    active[e] = false;
    componentMask[e] = COMP_NONE;
}

bool ECSWorld::isValid(Entity e) {
   return  e < MAX_ENTITIES && active[e];
}

bool ECSWorld::hasComponent(Entity e, uint32_t flag) const {
    return (componentMask[e] & flag) == flag;
}

void ECSWorld::addComponent(Entity e, uint32_t flag) {
    if (!isValid(e)) return;
    componentMask[e] |= flag;
}

void ECSWorld::removeComponent(Entity e, uint32_t flag) {
    if (!isValid(e)) return;
    componentMask[e] &= ~flag;
}
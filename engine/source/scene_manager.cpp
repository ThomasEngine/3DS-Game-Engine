#include "scene_manager.h"

#include "core.h"
#include "scene.h"

SceneManager::~SceneManager() {
    if (current) {
        current->exit();
        delete current;
        current = nullptr;
    }
    if (pending) {
        delete pending;
        pending = nullptr;
    }
}

void SceneManager::changeScene(BaseScene* next) {
    // store the request
    // if a pending scene already exists, delete it.
    if (pending && pending != next) {
        delete pending;
    }
    pending = next;
    hasPending = true;
}

void SceneManager::applyPendingSwitch() {
    if (!hasPending) return;

    // exit and destroy old scene
    if (current) {
        current->exit();
        delete current;
    }

    // activate new scene
    current = pending;
    pending = nullptr;
    hasPending = false;

    settings = engine_default_settings;

    if (current) {
        current->setManager(this);
        current->configurateSettings(settings);
        current->enter();
    }
}

void SceneManager::update(float dt) {
    // apply any pending switch BEFORE updating
    applyPendingSwitch();

    if (current) {
        current->update(dt);
    }
}

void SceneManager::renderTop(Renderer& renderer) {
    if (current) {
        current->renderTop(renderer);
    }
}

void SceneManager::renderBottom(Renderer& renderer) {
    if (current) {
        current->renderBottom(renderer);
    }
}

void SceneManager::setDefaultSettings(EngineSettings& s) {
    engine_default_settings = s;
    settings = s;
}
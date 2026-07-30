#include "core.h"
#include "game_scene.h"

int main() {
    EngineSettings settings;
    settings.useTileSnapping = true;
    settings.useFriction = false;
    settings.entityScale = 2.0f;

    Engine  engine;
    engine.setDefaultSettings(settings);
    engine.setScene(new GameScene());
    engine.run();
    return 0;
}


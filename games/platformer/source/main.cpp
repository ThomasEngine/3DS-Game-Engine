#include "core.h"
#include "main_scene.h"

int main() {
    EngineSettings defaultSettings;
    defaultSettings.tileScale = 2;
    defaultSettings.entityScale = 1;

    Engine engine;
    // engine.setDefaultSettings(defaultSettings);
    engine.setScene(new MainScene());
    engine.run();
    return 0;
}
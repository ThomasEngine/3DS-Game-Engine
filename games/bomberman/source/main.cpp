#include "core.h"
#include "game_scene.h"

int main() {
    Engine  engine;
    engine.setScene(new GameScene());
    engine.run();
    return 0;
}


#include "core.h"
#include "main_scene.h"

int main() {
    Engine engine;
    engine.setScene(new MainScene());
    engine.run();
    return 0;
}
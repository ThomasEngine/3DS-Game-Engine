#include "core.h"
#include "game_app.h"

int main() {
    GameApp app;
    Engine  engine(&app);
    engine.run();
    return 0;
}


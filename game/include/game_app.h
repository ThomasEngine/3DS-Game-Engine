#pragma once
//#include "ecs.h"  // TODO: implement ECS later
#include <3ds.h>
#include "graphics.h"

class GameApp {
private:
   //ECS_World* world;      // TODO: implement when ECS is ready
   GraphicsAssets* assets;
   
   // Game systems
   //class Player* player;
   class Level* level;
    
public:
    GameApp();
    ~GameApp();
    
    bool init();
    void update();
    void render();
    void shutdown();
    
    // Static callbacks for C interface
    static bool s_init(void* user_data);
    static void s_update(void* user_data);
    static void s_render(void* user_data);
    static void s_shutdown(void* user_data);
};
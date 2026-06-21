#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

enum Screens
{
    Top,
    Bottom
};

class Renderer
{
public:
    Renderer() {};
    ~Renderer() {};
    
    bool init();
    bool shutdown();
    
    void beginFrame();
    void endFrame();
    
    void beginTopScreen();
    void beginBottomScreen();
    
    void drawRect(float x, float y, float w, float h, u32 color);
    void drawText(const char* text, float x, float y, u32 color, float scale = 1.0f);
    
    void clearTop(u32 color);
    void clearBottom(u32 color);
    
    
private:
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
    C2D_TextBuf       text_buf;
    
};
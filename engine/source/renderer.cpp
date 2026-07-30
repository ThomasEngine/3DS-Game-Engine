#include "renderer.h"

bool Renderer::init() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    romfsInit();

    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    //consoleInit(GFX_BOTTOM, nullptr);

    // set bottom to console

    text_buf = C2D_TextBufNew(4096);

    return true;
}

bool Renderer::shutdown() {
    C2D_TextBufDelete(text_buf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
    return true;
}

void Renderer::beginFrame() {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
}

void Renderer::endFrame() {
    C3D_FrameEnd(0);
}

void Renderer::beginTopScreen() {
    C2D_TargetClear(top, C2D_Color32(0, 0, 0, 255));
    C2D_SceneBegin(top);
}

void Renderer::beginBottomScreen() {
    C2D_TargetClear(bottom, C2D_Color32(0, 0, 0, 255));
    C2D_SceneBegin(bottom);
}

void Renderer::drawRect(float x, float y, float w, float h, u32 color) {
    C2D_DrawRectSolid(x, y, 0, w, h, color);
}

void Renderer::drawText(const char *text, float x, float y, u32 color, float scale) {
    C2D_TextBufClear(text_buf); // clear before reusing

    C2D_Text c2dText;
    C2D_TextParse(&c2dText, text_buf, text);
    C2D_TextOptimize(&c2dText);
    C2D_DrawText(&c2dText, C2D_WithColor, x, y, 0, scale, scale, color);
}

void Renderer::clearTop(u32 color) {
    C2D_TargetClear(top, color);
}

void Renderer::clearBottom(u32 color) {
    C2D_TargetClear(bottom, color);
}

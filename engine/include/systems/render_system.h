#pragma once
#include "ecs.h"
#include "renderer.h"

namespace systems {
    void render_draw(ECSWorld& world, Renderer& renderer, RenderLayer layer, const EngineSettings& settings);
}




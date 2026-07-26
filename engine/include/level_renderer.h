#pragma once
#include <functional>
#include <citro2d.h>
#include <citro3d.h>

#include "engine_settings.h"

using TileSpriteCallback = std::function<C2D_Image(int x, int y)>;

// const int TILE_SIZE = TILE_SIZE_PX;

class Camera;

void draw_level(int width, int height, const Camera& camera, const TileSpriteCallback& getSprite, const EngineSettings& settings);
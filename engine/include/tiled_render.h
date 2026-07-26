#pragma once

#include "engine_settings.h"
#include "tiled_loader.h"
#include "renderer.h"

// strip tiled flip bits and apply GID offset (-1 if empty)
int tile_index_from_gid(uint32_t gid);

// draw all tile layers of map with camera + per layer parallax
void draw_tiled_map(const TiledMap& map, const Camera& cam, C2D_SpriteSheet tileSheet, const EngineSettings& settings);
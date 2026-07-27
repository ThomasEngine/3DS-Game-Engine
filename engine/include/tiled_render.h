#pragma once

#include "engine_settings.h"
#include "tiled_loader.h"
#include "renderer.h"

constexpr uint32_t TILE_FLIP_H = 0x80000000u;
constexpr uint32_t TILE_FLIP_V = 0x40000000u;
constexpr uint32_t TILE_FLIP_D = 0x20000000u;
constexpr uint32_t TILE_FLIP_MASK = TILE_FLIP_H | TILE_FLIP_V | TILE_FLIP_D;

int  tile_index_from_gid(uint32_t gid);
inline bool tile_flipped_h(uint32_t gid) { return (gid & TILE_FLIP_H) != 0; }
inline bool tile_flipped_v(uint32_t gid) { return (gid & TILE_FLIP_V) != 0; }

void draw_tiled_map(const TiledMap& map, const Camera& cam, C2D_SpriteSheet tileSheet, const EngineSettings& settings);

#pragma once

// This file defines things that the engine uses. You can enable and disable stuff.

// MOVEMENT
// Apply friction to movement with velocity
constexpr bool BFRICTION = false;

// GRAVITY
constexpr float GRAVITY        = 9.8f;
constexpr float MAX_FALL_SPEED = 20.0f;

constexpr int TILE_SIZE_PX = 16;
constexpr float TILE_SCALE = 2.0f;
constexpr float ENTITY_SCALE = 2.0f;
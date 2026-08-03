# Code Style 3DS Game Engine

This document describes the naming conventions, structural rules, and design patterns used in this engine. Contributors should follow these guidelines to keep the codebase consistent.

## Naming conventions

| Element | Convention | Example |
|---------|-----------|---------|
| Namespaces | `lowercase` (single word) | `input`, `audio`, `tiled`, `background`, `camera`, `systems`, `ecs` |
| Free functions inside namespaces | `snake_case` | `input::action_pressed`, `audio::load`, `tiled::draw_map` |
| Classes | `PascalCase` | `Engine`, `Renderer`, `SceneManager`, `BaseScene`, `Sprite`, `Level` |
| Structs (data types) | `PascalCase` | `AudioClip`, `Background`, `Camera`, `Animation`, `Vec2` |
| Class/struct methods | `camelCase` | `Sprite::playAnimation`, `Level::isSolid`, `SceneManager::changeScene` |
| Member variables | `camelCase` | `sceneManager`, `frameCount`, `currentAnim` |
| Constants and enum values | `UPPER_SNAKE_CASE` | `MAX_ENTITIES`, `TILE_SIZE_PX`, `LAYER_TOP` |
| Enum class values | `UPPER_SNAKE_CASE` (scoped, no prefix needed) | `input::Action::JUMP`, `RenderLayer::TOP` |
| File names | `snake_case.h` / `snake_case.cpp` | `tiled_loader.cpp`, `main_scene.h`, `game_assets.cpp` |
| Component structs | `PascalCase` ending in `Component` | `PositionComponent`, `VelocityComponent`, `GravityComponent` |
| Component flags | `UPPER_SNAKE_CASE` with `COMP_` prefix | `COMP_POSITION`, `COMP_VELOCITY` (may drop prefix if moved into `ecs::` namespace) |

## Structural rules

### Namespaces
- Module-level namespaces group related free functions: `input::`, `audio::`, `tiled::`, `background::`, `camera::`, `systems::`, `ecs::`
- Core engine classes (`Engine`, `Renderer`, `SceneManager`, `BaseScene`) live at global scope — not wrapped in an outer engine namespace
- "Vocabulary" types used broadly (`Camera`, `Sprite`, `Animation`, `Background`, `Vec2`) live at global scope; the module's functions are namespaced
- Module-internal types live inside their namespace: `input::Action`, `audio::AudioClip`, `tiled::Map`, `tiled::Layer`, `tiled::Object`
- Never use `using namespace X;` in a header file
- Use anonymous namespaces (`namespace { ... }`) inside `.cpp` files to hide private state and helpers

### Includes
- Use `#pragma once` at the top of headers, not include guards
- Include order in `.cpp`: matching header first, then own project headers, then third-party, then standard library
- Avoid relative includes like `../include/foo.h` rely on the include path

### Files
- One class or one logical module per file
- Header (`.h`) and source (`.cpp`) mirror each other
- File name reflects its primary contents (`tiled_loader.cpp` for the loader, `tiled_render.cpp` for the renderer)

### Functions
- Free functions in namespaces should read as verb phrases: `audio::play`, `tiled::load`, `input::bind`
- Prefer more specific names to avoid stdlib collisions, `unload` instead of `free`, `read_file` instead of `read`
- Const-correctness: mark methods `const` if they don't modify state; pass parameters by `const&` when not modified

## Language and code rules

### C++ standard
- Target C++17
- Use `enum class` for scoped, type-safe enumerations
- Use `constexpr` for compile-time constants
- Use `auto` only for, for loops
- Use references over pointers when the value is required and never null; pointers for optional/nullable references

### Data-oriented style
- Fixed-size arrays over dynamic containers in hot paths (per-entity data)
- `std::vector` is fine for one-time-load data (levels, assets, particles pool)
- No exceptions — the engine compiles with `-fno-exceptions`
- No RTTI — the engine compiles with `-fno-rtti`
- No STL streams for I/O; use `fopen`/`fread`/`fwrite`

### Memory
- Prefer stack allocation and fixed pools; heap allocation is fine but be aware of it
- Audio and other DSP-visible data must use `linearAlloc` / `linearFree`
- Match every `new` with `delete`, every `linearAlloc` with `linearFree`

### Coordinates and units
- Positions are in tiles, not pixels (floats — a tile is 1.0 wide, half-tiles at 0.5 offsets)
- Time is in seconds (`dt` is a float in seconds)
- Angles in radians
- Colors are `uint32_t` in RGBA order (`0xRRGGBBAA` format)
- Screen dimensions: top screen is 400x240 pixels, bottom is 320x240

## Design patterns

### Module design
- Every module has a `mod::init()` / `mod::exit()` if it owns resources
- Modules can also have `mod::update(dt)` for per-frame maintenance
- File-level state (in anonymous namespace) is fine — one instance per module, similar to the input module

### ECS
- Positions in tile coordinates; entities may reside at tile centers (`.5, .5`) or grid-aligned depending on genre
- Component structs are plain data — no methods, no constructors that do work
- Systems are free functions in the `systems::` namespace that iterate entities and operate on components
- Components are added with `world.addComponent(entity, COMP_A | COMP_B)`; systems guard with `if (!world.hasComponent(entity, COMP_X)) continue;`

### Rendering
- Rendering is done via citro2d
- Tile-based games use Tiled `.tmj` files loaded through `tiled::load`
- Levels contain both tile layers (rendering + collision) and object layers (spawn points, triggers)
- Cameras track a target entity with configurable dead zone, lookahead, damping, and shake

### Scenes
- Scenes derive from `BaseScene`
- Scenes have access to `renderer` and `manager` (SceneManager) as protected members
- Scenes override `enter()`, `exit()`, `update(dt)`, `renderTop()`, `renderBottom()`
- Scene transitions are deferred to the next frame (safe to call from within a scene's own methods)

## Formatting

- 4-space indentation, no tabs
- Braces on same line for functions, methods, and control flow (`{` at end of line)
- Space around binary operators (`a + b`, not `a+b`)
- Space after commas
- No trailing whitespace
- Line length: soft limit around 100 characters; hard limit around 120
- Blank line between logical sections of a function
- One blank line between function definitions in a `.cpp` file

## Comments

- Comments explain **why**, not **what**
- Public API functions get brief comments in the header describing intent, parameters, return value, and side effects
- No comment is better than a wrong comment
- `// TODO:` for planned work, `// FIXME:` for known bugs, `// NOTE:` for important context
- File-level headers are optional and short; don't write auto-generated boilerplate

## Suggested project structure

```
engine/
├── include/                  # Public engine headers
│   ├── core.h                # Engine, BaseScene
│   ├── renderer.h            # Renderer, Camera
│   ├── input.h               # input namespace
│   ├── audio.h               # audio namespace
│   ├── ecs.h                 # ecs::World, components
│   ├── sprite.h
│   ├── background.h          # Background struct + background namespace
│   ├── tiled_loader.h        # tiled::load, tiled::Map, etc.
│   ├── tiled_render.h        # tiled::draw_map
│   ├── camera.h              # camera::update, camera::shake
│   ├── engine_settings.h
│   └── systems/              # System headers
│       ├── physics_system.h
│       ├── collision_system.h
│       ├── render_system.h
│       └── animation_system.h
├── source/                   # Engine implementations mirroring include/
│   ├── core.cpp
│   ├── ...
│   └── systems/
│       ├── physics_system.cpp
│       └── ...
└── external/
    └── cjson/                # Third-party libraries

games/
├── platformer/
│   ├── include/              # Per-game headers
│   ├── source/               # Per-game sources
│   ├── gfx/                  # Source art + sprites.json
│   └── romfs/                # Runtime assets (packed by build)
├── bomberman/
└── ...

tools/
├── slice.py                  # Sprite slicer
└── ...
```
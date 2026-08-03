# 3DS-Game-Engine

**Status:** In active development. Core engineis stable templates and tooling are still being worked on. Not ready for production.

## Features
- Entity-Component_System with fixed size array
- Scene management
- Tiled map loading ('.tmj' format) with layer parallex drawing
- 2D rendering via citro2d, with camera and frustum culling
- Sprite animation system
- Mutli-game architectur (one engine, multiple game projects)
- JSON sprite slicing and asset pipeline

## Requirements
- **devkitPro** with 3DS toolchain (`libctru`, `citro2d`, `citro3d`)
- **Python 3** with Pillow (`pip install Pillow`) for the sprite slicer
- **[Tiled](https://www.mapeditor.org/)** for editing levels
- **[Citra](https://citra-emu.org/)** or a modded 3DS for running

For installing devkitPro, follow this guide:
[3DS Homebrew Development Getting Started](https://gbatemp.net/threads/3ds-homebrew-development-getting-started-guide.666095/)


## Building
make GAME=<name>
Output goes to `build/<name>/<name>.3dsx`

## Sending to 3ds
You need homebrew installed on your 3ds.
You can then put it on the sd card, or do this:
On the 3ds homebrew launcher press Y. This will switch into netloader. Then replace the ip you see on the screen to the one below:
3dslink build/<name>/<name>.3dsx -a <ip-adress-of-3ds>

## Credits

- [devkitPro](https://devkitpro.org/) 3DS toolchain and libraries
- [cJSON](https://github.com/DaveGamble/cJSON) JSON parsing (MIT)

## License

MIT — see `LICENSE`.

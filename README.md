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
- **[Azahar](https://azahar-emu.org/)** or a modded 3DS for running
  (Citra was discontinued in 2024; Azahar is its actively maintained successor
  and loads `.3dsx` files directly)

## Setup

Install devkitPro following the official guide:
[devkitPro Getting Started](https://devkitpro.org/wiki/Getting_Started).
The 3DS toolchain (the `3ds-dev` package group) includes `libctru`, `citro3d`
and `citro2d`.

**Windows** — use the graphical installer and select 3DS development. It bundles
an MSYS2 environment, sets `DEVKITPRO`/`DEVKITARM` automatically, and adds an
*MSys2* shortcut to the Start menu — build from that shell. To add the 3DS
toolchain to an existing install: `pacman -S 3ds-dev` (plain `pacman`; the
`dkp-` prefix is only used on Linux/macOS).

**Linux** — install devkitPro pacman per the guide above, then:

```sh
sudo dkp-pacman -S 3ds-dev
```

**macOS** — see [macOS-build.md](macOS-build.md) for verified step-by-step
instructions, including Apple Silicon notes and known Gatekeeper issues.

On Linux and macOS the `Makefile` needs `DEVKITARM` in your environment (it
refuses to run without it), and the installer does not reliably export it to
your shell — add this to your shell profile (`~/.bashrc`, `~/.bash_profile`
or `~/.zshrc`):

```sh
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=${DEVKITPRO}/devkitARM
export DEVKITPPC=${DEVKITPRO}/devkitPPC
export PATH=${DEVKITPRO}/tools/bin:$PATH
```

On Windows this is not needed — the installer sets the variables for the MSYS2
shell.

## Building

Run from the repository root:

```sh
make GAME=<name>
```

Available games are the folders under `games/`: `bomberman`, `platformer`, `blank`.
Output goes to `build/<name>/<name>.3dsx`.

## Sending to 3ds
You need homebrew installed on your 3ds.
You can then put it on the sd card, or do this:
On the 3ds homebrew launcher press Y. This will switch into netloader. Then replace the ip you see on the screen to the one below:
`3dslink build/<name>/<name>.3dsx -a <3ds-ip>`

## Credits

- [devkitPro](https://devkitpro.org/) 3DS toolchain and libraries
- [cJSON](https://github.com/DaveGamble/cJSON) JSON parsing (MIT)

## License

MIT — see `LICENSE`.

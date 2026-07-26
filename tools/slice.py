import os
import sys
import json
import subprocess
import math
from PIL import Image

TEX3DS = r"C:\devkitPro\tools\bin\tex3ds.exe"


# ---------------------------------------------------------------------------
# Shared helpers
# ---------------------------------------------------------------------------

def write_t3s(t3s_path, out_subdir, filenames):
    """Write a .t3s file listing the given sliced PNG filenames."""
    lines = [
        "-f rgba8888",
        "-z auto",
        "--atlas",
        ""  # blank line before file list
    ]
    for fn in filenames:
        lines.append(f"{out_subdir}/{fn}")
    with open(t3s_path, "w") as f:
        f.write("\n".join(lines) + "\n")


def convert_t3s(t3s_path, out_t3x):
    """Run tex3ds to convert a .t3s into a .t3x."""
    result = subprocess.run(
        [TEX3DS, "-i", t3s_path, "-o", out_t3x],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        print(f"  ERROR: {result.stderr.strip()}")
        return False
    return True


def build_tileset_image(sheet_def, gfx_dir, tile_images, sheet_name):
    """Combine sliced tiles into a single tileset image for Tiled."""
    cell = sheet_def["cell_size"]
    num_tiles = len(tile_images)
    if num_tiles == 0:
        return

    layout = sheet_def.get("tileset_layout", "row")

    if layout == "square":
        cols = int(math.ceil(math.sqrt(num_tiles)))
        rows = int(math.ceil(num_tiles / cols))
    else:  # "row" or anything else defaults to row
        cols = num_tiles
        rows = 1

    out_w = cols * cell
    out_h = rows * cell
    tileset = Image.new("RGBA", (out_w, out_h), (0, 0, 0, 0))

    for i, img in enumerate(tile_images):
        col = i % cols
        row = i // cols
        tileset.paste(img, (col * cell, row * cell))

    tiled_dir = os.path.join(gfx_dir, "tiled")
    os.makedirs(tiled_dir, exist_ok=True)
    out_path = os.path.join(tiled_dir, f"{sheet_name}_tileset.png")
    tileset.save(out_path)
    print(f"  tileset image: tiled/{sheet_name}_tileset.png ({cols}x{rows})")


# ---------------------------------------------------------------------------
# Slicing modes
# ---------------------------------------------------------------------------

def slice_grid(sheet_def, gfx_dir, sliced_dir, out_subdir):
    """Slice a source PNG into a full grid of cells.
    Returns (filenames, frame_info, tile_images)."""
    cell = sheet_def["cell_size"]
    img_path = os.path.join(gfx_dir, sheet_def["file"])
    img = Image.open(img_path)

    cols = img.width  // cell
    rows = img.height // cell

    filenames = []
    tile_images = []
    index = 0
    for row in range(rows):
        for col in range(cols):
            x = col * cell
            y = row * cell
            crop = img.crop((x, y, x + cell, y + cell))
            name = f"tile_{index}.png"
            crop.save(os.path.join(sliced_dir, name))
            filenames.append(name)
            tile_images.append(crop)
            index += 1

    print(f"  grid: sliced {index} tiles ({cols}x{rows})")
    return filenames, {}, tile_images


def slice_sparse(sheet_def, gfx_dir, sliced_dir, out_subdir):
    """Slice specific cells from a source PNG by (col, row).
    Supports multiple frames per entry (frames continue horizontally).
    Returns (filenames, frame_info, tile_images)."""
    cell = sheet_def["cell_size"]
    img_path = os.path.join(gfx_dir, sheet_def["file"])
    img = Image.open(img_path)

    filenames = []
    frame_info = {}
    tile_images = []

    for entry in sheet_def["entries"]:
        name   = entry["name"]
        col    = entry["col"]
        row    = entry["row"]
        frames = entry.get("frames", 1)

        for frame in range(frames):
            x = (col + frame) * cell
            y = row * cell
            crop = img.crop((x, y, x + cell, y + cell))

            if frames > 1:
                out_name = f"{name}_{frame}.png"
            else:
                out_name = f"{name}.png"

            crop.save(os.path.join(sliced_dir, out_name))
            filenames.append(out_name)
            tile_images.append(crop)

        if frames > 1:
            frame_info[name] = frames

    print(f"  sparse: sliced {len(filenames)} images")
    return filenames, frame_info, tile_images


def slice_strips(sheet_def, gfx_dir, sliced_dir, out_subdir):
    """Slice horizontal animation strips into frames.
    Returns (filenames, frame_info, tile_images)."""
    cell = sheet_def["cell_size"]
    filenames = []
    frame_info = {}
    tile_images = []

    for strip in sheet_def["strips"]:
        name = strip["name"]
        img_path = os.path.join(gfx_dir, strip["file"])
        img = Image.open(img_path)

        frames = img.width // cell
        for i in range(frames):
            x = i * cell
            crop = img.crop((x, 0, x + cell, img.height))
            out_name = f"{name}_{i}.png"
            crop.save(os.path.join(sliced_dir, out_name))
            filenames.append(out_name)
            tile_images.append(crop)

        frame_info[name] = frames
        print(f"  strip '{name}': {frames} frames")

    return filenames, frame_info, tile_images


def convert_backgrounds(backgrounds, gfx_dir, romfs_gfx):
    """Convert each background to its own .t3x (not atlased)."""
    for bg in backgrounds:
        name = os.path.splitext(bg)[0]
        img_path = os.path.join(gfx_dir, bg)
        if not os.path.exists(img_path):
            print(f"  background missing: {bg}")
            continue

        # write a single-image .t3s (no atlas for full backgrounds)
        t3s_path = os.path.join(gfx_dir, f"bg_{name}.t3s")
        with open(t3s_path, "w") as f:
            f.write("-f rgba8888\n-z auto\n\n")
            f.write(f"{bg}\n")

        out_t3x = os.path.join(romfs_gfx, f"{name}.t3x")
        if convert_t3s(t3s_path, out_t3x):
            print(f"  background: {name}.t3x")


# ---------------------------------------------------------------------------
# Per-game slicing
# ---------------------------------------------------------------------------

def slice_game(game):
    gfx_dir   = f"games/{game}/gfx"
    config    = f"{gfx_dir}/sprites.json"
    romfs_gfx = f"games/{game}/romfs/gfx"

    if not os.path.exists(config):
        print(f"No sprites.json found in {gfx_dir}")
        return

    with open(config, encoding="utf-8-sig") as f:
        data = json.load(f)

    out_subdir = data.get("output_dir", "sliced")
    sliced_dir = os.path.join(gfx_dir, out_subdir)
    os.makedirs(sliced_dir, exist_ok=True)
    os.makedirs(romfs_gfx, exist_ok=True)

    all_frame_info = {}

    # --- process each sheet ---
    sheets = data.get("sheets", {})
    for sheet_name, sheet_def in sheets.items():
        mode = sheet_def.get("mode", "grid")
        print(f"Sheet '{sheet_name}' (mode: {mode})")

        if mode == "grid":
            filenames, frame_info, tile_images = slice_grid(sheet_def, gfx_dir, sliced_dir, out_subdir)
        elif mode == "strips":
            filenames, frame_info, tile_images = slice_strips(sheet_def, gfx_dir, sliced_dir, out_subdir)
        elif mode == "sparse":
            filenames, frame_info, tile_images = slice_sparse(sheet_def, gfx_dir, sliced_dir, out_subdir)
        else:
            print(f"  unknown mode '{mode}', skipping")
            continue

        # record any animation frame info
        if frame_info:
            all_frame_info[sheet_name] = frame_info

        # optionally build a Tiled-ready tileset image
        if sheet_def.get("build_tileset", False):
            build_tileset_image(sheet_def, gfx_dir, tile_images, sheet_name)

        # write .t3s and convert to .t3x
        t3s_path = os.path.join(gfx_dir, f"{sheet_name}.t3s")
        write_t3s(t3s_path, out_subdir, filenames)
        out_t3x = os.path.join(romfs_gfx, f"{sheet_name}.t3x")
        if convert_t3s(t3s_path, out_t3x):
            print(f"  -> {sheet_name}.t3x")

    # --- process backgrounds ---
    backgrounds = data.get("backgrounds", [])
    if backgrounds:
        print("Backgrounds:")
        convert_backgrounds(backgrounds, gfx_dir, romfs_gfx)

    # --- write frame info for reference ---
    if all_frame_info:
        info_path = os.path.join(gfx_dir, "frame_info.json")
        with open(info_path, "w") as f:
            json.dump(all_frame_info, f, indent=2)
        print(f"\nFrame info written to {info_path}")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def list_games():
    return [d for d in os.listdir("games")
            if os.path.isdir(os.path.join("games", d))]


def main():
    games = list_games()
    if not games:
        print("No games found")
        return

    print("=== Sprite Slicer ===\n")
    print("Available games:")
    for i, g in enumerate(games):
        print(f"  {i + 1}. {g}")
    print(f"  {len(games) + 1}. ALL")

    choice = input("\nChoose a game (number): ").strip()
    try:
        idx = int(choice) - 1
    except ValueError:
        print("Invalid choice")
        return

    if idx == len(games):
        for g in games:
            print(f"\n--- {g} ---")
            slice_game(g)
    elif 0 <= idx < len(games):
        print(f"\n--- {games[idx]} ---")
        slice_game(games[idx])
    else:
        print("Invalid choice")
        return

    print("\nDone! Rebuild your game to bundle the new romfs.")


if __name__ == "__main__":
    main()
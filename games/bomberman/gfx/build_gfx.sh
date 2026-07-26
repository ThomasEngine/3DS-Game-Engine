#!/bin/bash
# Convert PNG files to .t3x using tex3ds

set -e

GFXDIR="gfx"
CONVERTED="$GFXDIR/converted"

mkdir -p "$CONVERTED"

echo "Converting graphics..."

# Sprites
if [ -f "$GFXDIR/sprites/player.png" ]; then
    magick convert "$GFXDIR/sprites/player.png" -define colorspace:auto-grayscale=off "$CONVERTED/player_temp.png"
    tex3ds -i "$GFXDIR/sprites.t3s" -o "$CONVERTED/sprites.t3x"
fi

# Tiles
if [ -f "$GFXDIR/sprites/tiles.png" ]; then
    magick convert "$GFXDIR/sprites/tiles.png" "$CONVERTED/tiles_temp.png"
    tex3ds -i "$GFXDIR/tiles.t3s" -o "$CONVERTED/tiles.t3x"
fi

echo "Graphics conversion complete!"
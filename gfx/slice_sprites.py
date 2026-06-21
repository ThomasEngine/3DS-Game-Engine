from PIL import Image
import os

sheet = Image.open('sprites/SpriteSheet.png')

os.makedirs('sprites/sliced', exist_ok=True)
os.makedirs('tiles/sliced', exist_ok=True)

SPRITE_W = 16
SPRITE_H = 16

def slice_sprite(col, row, name, folder='sprites/sliced'):
    x = col * SPRITE_W
    y = row * SPRITE_H
    sprite = sheet.crop((x, y, x + SPRITE_W, y + SPRITE_H))
    sprite.save(f'{folder}/{name}.png')
    print(f'Saved {name}.png')

# Player sprites (already done)
slice_sprite(0, 0, 'player_left_0')
slice_sprite(1, 0, 'player_left_1')
slice_sprite(2, 0, 'player_left_2')
slice_sprite(3, 0, 'player_down_0')
slice_sprite(4, 0, 'player_down_1')
slice_sprite(5, 0, 'player_down_2')
slice_sprite(6, 0, 'player_right_0')
slice_sprite(7, 0, 'player_right_1')
slice_sprite(8, 0, 'player_right_2')
slice_sprite(9, 0,  'player_up_0')
slice_sprite(10, 0, 'player_up_1')
slice_sprite(11, 0, 'player_up_2')

# Tiles
slice_sprite(3, 3, 'tile_wall',       'tiles/sliced')
slice_sprite(4, 3, 'tile_breakable',  'tiles/sliced')
slice_sprite(3, 4, 'tile_floor',      'tiles/sliced')
slice_sprite(4, 4, 'tile_floor_alt',  'tiles/sliced')

print('Done!')
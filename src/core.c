#include <stdio.h>
#include "core.h"

uint8_t *debug = (void *)0xB000;

const uint8_t map_tile_lookup[64] = {
  0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
  0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E,
  0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C, 0x4E,
  0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E,
  0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C, 0x8E,
  0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE,
  0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
  0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
};

uint8_t flags[32] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const palette_color_t blank_palette[4] = {
  RGB_WHITE,
  RGB_WHITE,
  RGB_WHITE,
  RGB_WHITE,
};

/**
 * Font tileset.
 */
const Tileset tileset_font = { 128, 10, tile_data_font };

/**
 * Battle tileset.
 */
const Tileset tileset_battle = { 5 * 16, 10, tile_battle };

/**
 * Common objects tileset.
 */
const Tileset objects_tileset = { 128, 10, tile_data_objects };

/**
 * Dungeon level tileset (page 1).
 */
const Tileset dungeon_tileset_page1 = {
  128,
  12,
  tile_data_dungeon
};

/**
 * Dungeon level tileset (page 2).
 */
const Tileset dungeon_tileset_page2 = {
  128,
  12,
  tile_data_dungeon + BYTES_PER_TILE * 128
};

/**
 * Dungeon level tileset (page 2).
 */
const Tileset dungeon_tileset_page3 = {
  128,
  12,
  tile_data_dungeon + BYTES_PER_TILE * 128 * 2
};

/**
 * Hero sprites tileset.
 */
const Tileset hero_tileset = { 12 * 8, 10, tile_data_hero };

/**
 * Monster map character tiles, page1.
 */
const Tileset monsters_tileset_page1 = {
  96, 15, tile_monsters
};

/**
 * Monster map character tiles, page2.
 */
const Tileset monsters_tileset_page2 = {
  96, 15, tile_monsters + BYTES_PER_TILE * 96
};

/**
 * Monster map character tiles, page2.
 */
const Tileset monsters_tileset_page3 = {
  96, 15, tile_monsters + BYTES_PER_TILE * 96 * 2
};

void load_tileset(const Tileset *s, uint8_t *dst) NONBANKED {
  const uint8_t *src = s->data;
  uint8_t size = s->size;
  uint8_t _prev_bank = _current_bank;
  SWITCH_ROM(s->bank);

  do {
    for (uint8_t k = 0; k < BYTES_PER_TILE; k++)
      *dst++ = *src++;
  } while (--size);

  SWITCH_ROM(_prev_bank);
}

void core_load_tiles(
  const Tileset *s,
  uint8_t *dst,
  uint8_t o,
  uint8_t n
) NONBANKED {
  const uint8_t *src = s->data + o * BYTES_PER_TILE;
  uint8_t _prev_bank = _current_bank;
  SWITCH_ROM(s->bank);

  do {
    for (uint8_t k = 0; k < BYTES_PER_TILE; k++)
      *dst++ = *src++;
  } while (--n);

  SWITCH_ROM(_prev_bank);
}

void load_battle_tiles(void) NONBANKED {
  VBK_REG = VBK_BANK_1;
  load_tileset(&tileset_battle, VRAM_BATTLE_TILES);
}

void load_font(void) NONBANKED {
  VBK_REG = VBK_BANK_1;
  load_tileset(&tileset_font, VRAM_FONT_TILES);
}

void load_dungeon_tiles(void) NONBANKED {
  VBK_REG = VBK_BANK_0;
  core.load_tileset(&dungeon_tileset_page1, VRAM_BG_TILES);
  VBK_REG = VBK_BANK_0;
  core.load_tileset(&dungeon_tileset_page2, VRAM_SHARED_TILES);
  VBK_REG = VBK_BANK_1;
  core.load_tileset(&dungeon_tileset_page3, VRAM_BG_TILES);
}

void load_hero_tiles(uint8_t player_class) NONBANKED {
  const uint8_t row_width = 12;
  const uint8_t offset = player_class * row_width * 2;
  const uint8_t offset2 = offset + row_width;
  uint8_t *const vram = VRAM_SPRITE_TILES;
  uint8_t *const vram2 = vram + 16 * BYTES_PER_TILE;

  VBK_REG = VBK_BANK_0;
  core_load_tiles(&hero_tileset, vram, offset, row_width);
  core_load_tiles(&hero_tileset, vram2, offset2, row_width);
}

void load_all_heros(void) NONBANKED {
  const uint8_t row = 4 * 3;
  VBK_REG = VBK_BANK_0;
  for (uint8_t k = 0; k < 4; k++) {
    const uint8_t offset = k * row * 2;
    uint8_t *vram = VRAM_SPRITE_TILES + 0x20 * k;
    core_load_tiles(&hero_tileset, vram, offset, row);
    core_load_tiles(&hero_tileset, vram, offset + row, row);
  }
}

void load_object_tiles(void) NONBANKED {
  VBK_REG = VBK_BANK_1;
  load_tileset(&objects_tileset, VRAM_SPRITE_TILES);
}

void draw_tilemap(Tilemap m, uint8_t *dst) NONBANKED {
  uint8_t _prev_bank = _current_bank;
  SWITCH_ROM(m.bank);

  const uint8_t *src = m.data;
  for (uint8_t y = 0; y < m.height; y++, dst += 32 - m.width) {
    for (uint8_t x = 0; x < m.width; x++, dst++) {
      VBK_REG = VBK_TILES;
      *dst = *src++;
      VBK_REG = VBK_ATTRIBUTES;
      *dst = *src++;
    }
  }

  SWITCH_ROM(_prev_bank);
}

void load_bg_palette(const palette_color_t *data, uint8_t index, uint8_t n) {
  update_bg_palettes(index, n, data);
}

void load_sprite_palette(const palette_color_t *data, uint8_t index, uint8_t n) {
  update_sprite_palettes(index, n, data);
}

void clear_bg(void) NONBANKED {
  uint8_t *vram = VRAM_BACKGROUND;
  for (uint16_t k = 0; k < 32 * 32; k++) {
    VBK_REG = VBK_TILES;
    *vram = 0;
    VBK_REG = VBK_ATTRIBUTES;
    *vram++ = 0;
  }
}

void fill_bg(uint8_t tile_id, uint8_t attr) NONBANKED {
  uint8_t *vram = VRAM_BACKGROUND;
  for (uint16_t k = 0; k < 32 * 32; k++) {
    VBK_REG = VBK_TILES;
    *vram = tile_id;
    VBK_REG = VBK_ATTRIBUTES;
    *vram++ = attr;
  }
}

void draw_text(uint8_t *vram, const char *text, uint8_t max) NONBANKED {
  VBK_REG = VBK_TILES;
  while (*text != 0 && max) {
    set_vram_byte(vram++, (*text++) + FONT_OFFSET);
    max--;
  }
  while (max) {
    set_vram_byte(vram++, FONT_SPACE);
    max--;
  }
}

void print_fraction(uint8_t *vram, uint16_t n, uint16_t d) {
  char buf[10];
  sprintf(buf, "%u/%u", n, d);
  draw_text(vram, buf, 7);
}

void hide_sprites(void) {
  for (uint8_t k = 0; k < 40; k++)
    move_sprite(k, 0, 0);
}

void core_fill(
  uint8_t *vram,
  uint8_t w,
  uint8_t h,
  uint8_t tile,
  uint8_t attr
) {
  for (uint8_t y = 0; y < h; y++, vram += 32 - w) {
    for (uint8_t x = 0; x < w; x++, vram++) {
      VBK_REG = VBK_TILES;
      set_vram_byte(vram, tile);
      VBK_REG = VBK_ATTRIBUTES;
      set_vram_byte(vram, attr);
    }
  }
}

void load_monster_tiles(MonsterTiles tiles, MonsterTilePosition pos) {
  const Tileset *tileset;

  switch (tiles) {
  case MONSTER_TILES_KOBOLD:
  case MONSTER_TILES_GOBLIN:
  case MONSTER_TILES_ZOMBIE:
  case MONSTER_TILES_BUGBEAR:
    tileset = &monsters_tileset_page1;
    break;
  case MONSTER_TILES_OWLBEAR:
  case MONSTER_TILES_GELATINOUS_CUBE:
  case MONSTER_TILES_DISPLACER_BEAST:
  case MONSTER_TILES_WILL_O_WISP:
    tileset = &monsters_tileset_page2;
    break;
  default:
    tileset = &monsters_tileset_page3;
  }

  const uint8_t row_width = 12;
  const uint8_t offset = tiles * row_width * 2;
  const uint8_t offset2 = offset + row_width;

  uint8_t *vram = (void *)(0x8000 + BYTES_PER_TILE * (0x20 + 0x20 * pos));
  uint8_t *vram2 = vram + 16 * BYTES_PER_TILE;

  VBK_REG = VBK_BANK_0;
  core_load_tiles(tileset, vram, offset, row_width);
  core_load_tiles(tileset, vram2, offset2, row_width);
}

const Core core = {
  load_tileset,
  core_load_tiles,
  load_battle_tiles,
  load_font,
  load_hero_tiles,
  load_all_heros,
  load_object_tiles,
  draw_tilemap,
  load_bg_palette,
  load_sprite_palette,
  clear_bg,
  fill_bg,
  draw_text,
  print_fraction,
  hide_sprites,
  core_fill,
  load_dungeon_tiles,
  load_monster_tiles,
};

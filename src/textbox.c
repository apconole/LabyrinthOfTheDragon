#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>

#include "data.h"
#include "joypad.h"
#include "textbox.h"
#include "util.h"


TextboxState textbox_state = TEXTBOX_CLOSED;
Timer textbox_timer;
char *textbox_text;
uint8_t *textbox_vram;
uint8_t textbox_text_idx = 0;
uint8_t textbox_line = 0;
uint8_t textbox_y = 144;
uint8_t textbox_scroll_line = 0;

void init_text_box(void) {
  uint8_t t = 0;
  uint8_t a = TILEMAP_TEXTBOX_H * TILEMAP_TEXTBOX_W;
  for (uint8_t y = 0; y < TILEMAP_TEXTBOX_H; y++) {
    uint8_t *tile_addr = (void *)(0x9C00 + y * 0x20);
    for (uint8_t x = 0; x < TILEMAP_TEXTBOX_W; x++) {
      VBK_REG = VBK_TILES;
      set_vram_byte(tile_addr, tilemap_textbox[t++]);
      VBK_REG = VBK_ATTRIBUTES;
      set_vram_byte(tile_addr++, tilemap_textbox[a++]);
    }
  }
  textbox_y = 144;
  textbox_state = TEXTBOX_CLOSED;
  move_win(7, textbox_y);
}

void open_textbox(char *text) {
  textbox_text = text;
  textbox_text_idx = 0;
  textbox_state = TEXTBOX_OPENING;

  VBK_REG = VBK_TILES;

  uint8_t *clear_addr = (void *)(0x9C00 + 1 + 0x20);
  for (uint8_t y = 0; y < 4 ; y++) {
    for (uint8_t x = 0; x < 18; x++) {
      set_vram_byte(clear_addr++, 0xA0);
    }
    clear_addr += 14;
  }
}

void update_textbox(void) {
  uint8_t *scroll_vram = 0;
  uint8_t x = 0;

  switch (textbox_state) {
  case TEXTBOX_OPENING:
    move_win(7, textbox_y--);
    if (textbox_y == 96) {
      textbox_line = 0;
      textbox_state = TEXTBOX_CLEAR_WAIT;
      textbox_vram = (void *)(0x9C00 + 1 + 0x40);
      init_timer(textbox_timer, TEXTBOX_CLEAR_DELAY);
    }
    break;
  case TEXTBOX_CLOSING:
    move_win(7, textbox_y++);
    if (textbox_y == 144) {
      textbox_state = TEXTBOX_CLOSED;
    }
    break;
  case TEXTBOX_CLEAR:
    uint8_t *clear_addr = (void *)(0x9C00 + 1 + 0x20);
    for (uint8_t y = 0; y < 4 ; y++) {
      for (uint8_t x = 0; x < 18; x++) {
        set_vram_byte(clear_addr++, 0xA0);
      }
      clear_addr += 14;
    }
    textbox_line = 0;
    textbox_state = TEXTBOX_CLEAR_WAIT;
    textbox_vram = (void *)(0x9C00 + 1 + 0x40);
    init_timer(textbox_timer, TEXTBOX_CLEAR_DELAY);
    break;
  case TEXTBOX_CLEAR_WAIT:
    if (update_timer(textbox_timer)) {
      init_timer(textbox_timer, TEXTBOX_CHAR_DELAY);
      textbox_state = TEXTBOX_PRINTING;
    }
    break;
  case TEXTBOX_PRINTING:
    if (update_timer(textbox_timer)) {
      init_timer(textbox_timer, TEXTBOX_CHAR_DELAY);
      uint8_t c = textbox_text[textbox_text_idx];
      textbox_text_idx++;

      if (c == 0x00) {
        textbox_state = TEXTBOX_FINISHED;
      } else if (c == '\n') {
        textbox_line++;
        if (textbox_line == 1) {
          textbox_line++;
          textbox_vram = (void *)(0x9C00 + 1 + 0x20 * 4);
        } else {
          textbox_state = TEXTBOX_LINE_WAIT;
        }
      } else if (c == 0x03) {
        textbox_state = TEXTBOX_PAGE_WAIT;
      } else {
        set_vram_byte(textbox_vram++, c + 0x80);
      }
    }
    break;
  case TEXTBOX_SCROLL:
    if (update_timer(textbox_timer)) {
      init_timer(textbox_timer, TEXTBOX_SCROLL_DELAY);
      scroll_vram = (void *)(0x9C00 + 0x20 + 1);
      textbox_scroll_line++;

      if (textbox_scroll_line == 1) {
        for (uint8_t x = 0; x < 18; x++) {
          const uint8_t c0 = get_vram_byte(scroll_vram + 0x20);
          set_vram_byte(scroll_vram, c0);
          set_vram_byte(scroll_vram + 0x20, 0xA0);
          const uint8_t c1 = get_vram_byte(scroll_vram + 0x60);
          set_vram_byte(scroll_vram + 0x40, c1);
          set_vram_byte(scroll_vram + 0x60, 0xA0);
          scroll_vram++;
        }
      } else if (textbox_scroll_line == 2) {
        for (uint8_t x = 0; x < 18; x++) {
          set_vram_byte(scroll_vram, 0xA0);
          const uint8_t c = get_vram_byte(scroll_vram + 0x40);
          set_vram_byte(scroll_vram + 0x20, c);
          set_vram_byte(scroll_vram + 0x40, 0xA0);
          scroll_vram++;
        }
      } else {
        init_timer(textbox_timer, TEXTBOX_CHAR_DELAY);
        textbox_vram = (void *)(0x9C00 + 1 + 0x20 * 4);
        textbox_state = TEXTBOX_PRINTING;
      }
    }
    break;
  case TEXTBOX_LINE_WAIT:
    if (was_pressed(J_A | J_B)) {
      init_timer(textbox_timer, TEXTBOX_SCROLL_DELAY);
      textbox_scroll_line = 0;
      textbox_state = TEXTBOX_SCROLL;
    }
    break;
  case TEXTBOX_PAGE_WAIT:
    if (was_pressed(J_A | J_B)) {
      textbox_state = TEXTBOX_CLEAR;
    }
    break;
  case TEXTBOX_FINISHED:
    if (was_pressed(J_A | J_B)) {
      textbox_state = TEXTBOX_CLOSING;
    }
    break;
  }
}

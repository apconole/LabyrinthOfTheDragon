#include "core.h"
#include "sound.h"

#define SOUND_END 0

#define sweep(p, d, s) \
  ((((p) & 0x3) << 4) | (((d) & 1) << 3) | ((s) & 0x3))

#define envelope(i, d, p) \
  ((((i) & 0xF) << 4) | (((d) & 1) << 3) | ((p) & 0x03))

/**
 * Noise frequency.
 * @param shift Shift clock frequency.
 * @param steps Polynomial counter steps (0: 15, 1: 7).
 * @param ratio Dividing ratio frequency.
 */
#define noise_freq(shift, steps, ratio) \
  ((((shift) & 0xF) << 4) | ((steps & 0x1) << 3) | (ratio & 0x3))

typedef struct SoundRegister {
  uint8_t *address;
  uint8_t *data;
  bool enabled;
  Timer timer;
} SoundRegister;

inline void register_disable(SoundRegister *r) {
  r->enabled = false;
}

inline void register_next(SoundRegister *r) {
  if (!r->enabled)
    return;

  const uint8_t frames = *r->data++;
  if (!frames) {
    r->enabled = false;
    return;
  }
  init_timer(r->timer, frames);

  const uint8_t value = *r->data++;
  *r->address = value;
}

inline void register_update(SoundRegister *r) {
  if (!r->enabled)
    return;

  if (!update_timer(r->timer))
    return;

  register_next(r);
}

volatile SoundRegister nr10 = { (void *)0xFF10 };
volatile SoundRegister nr11 = { (void *)0xFF11 };
volatile SoundRegister nr12 = { (void *)0xFF12 };
volatile SoundRegister nr13 = { (void *)0xFF13 };
volatile SoundRegister nr14 = { (void *)0xFF14 };

volatile SoundRegister nr41 = { (void *)0xFF20 };
volatile SoundRegister nr42 = { (void *)0xFF21 };
volatile SoundRegister nr43 = { (void *)0xFF22 };
volatile SoundRegister nr44 = { (void *)0xFF23 };


void register_init(SoundRegister *r, uint8_t *data) {
  r->data = data;
  r->enabled = true;
  register_next(r);
}

Timer sound_update_timer;

void update_sound_isr(void) {
  if (!update_timer(sound_update_timer))
    return;
  reset_timer(sound_update_timer);

  register_update(&nr10);
  register_update(&nr11);
  register_update(&nr12);
  register_update(&nr13);
  register_update(&nr14);

  register_update(&nr41);
  register_update(&nr42);
  register_update(&nr43);
  register_update(&nr44);
}

void sound_init(void) {
  // Note: SO1 (left) and SO2 (right)
  // Enable sound
  NR52_REG = 0b10000000;
  // SO1 & SO2 on at maximum volume
  NR50_REG = 0b01110111;
  // Send channels 1, 2 & 4 to both SO1 & SO2
  NR51_REG = 0b11111111;

  init_timer(sound_update_timer, 4);
  add_TIM(update_sound_isr);
  TAC_REG = 0b00000110;
  set_interrupts(TIM_IFLAG | VBL_IFLAG);
}


#define SFX_STAIRS_DURATION 28

const uint8_t sfx_stairs_nr42[] = {
  SFX_STAIRS_DURATION, (6 << 4) | 1,
  SFX_STAIRS_DURATION, (5 << 4) | 1,
  SFX_STAIRS_DURATION, (4 << 4) | 1,
  SFX_STAIRS_DURATION, (3 << 4) | 1,
  SOUND_END
};

const uint8_t sfx_stairs_nr44[] = {
  SFX_STAIRS_DURATION, 0xC0,
  SFX_STAIRS_DURATION, 0xC0,
  SFX_STAIRS_DURATION, 0xC0,
  SFX_STAIRS_DURATION, 0xC0,
  SOUND_END
};

void sfx_stairs(void) {
  NR41_REG = 0x00;
  NR43_REG = (2 << 4)| 3;
  register_init(&nr42, sfx_stairs_nr42);
  register_init(&nr44, sfx_stairs_nr44);
}

void sfx_error(void) {
  NR11_REG = 0;
  NR12_REG = 0xA1;
  NR13_REG = 0x2C;
  NR14_REG = 0xC0;
}

void sfx_wall_hit(void) {
  NR10_REG = 0b00111011;
  NR11_REG = 0b10000000;
  NR12_REG = 0x71;
  NR13_REG = 0x62;
  NR14_REG = 0xC5;
}

#define SFX_MENU_MOVE_DURATION 2

const uint8_t sfx_menu_move_nr13[] = {
  SFX_MENU_MOVE_DURATION, 0x82,
  SFX_MENU_MOVE_DURATION, 0x9C,
  SFX_MENU_MOVE_DURATION, 0xAC,
  SFX_MENU_MOVE_DURATION, 0xC1,
  SOUND_END
};

const uint8_t sfx_menu_move_nr14[] = {
  SFX_MENU_MOVE_DURATION, 0xC7,
  SFX_MENU_MOVE_DURATION, 0xC7,
  SFX_MENU_MOVE_DURATION, 0xC7,
  SFX_MENU_MOVE_DURATION, 0xC7,
  SOUND_END
};

void sfx_menu_move(void) {
  NR10_REG = 0;
  NR11_REG = 0b01000000 | 50;
  NR12_REG = envelope(8, 0, 1);
  register_init(&nr13, sfx_menu_move_nr13);
  register_init(&nr14, sfx_menu_move_nr14);
}

#define SFX_NEXT_ROUND_DURATION 13

const uint8_t sfx_next_round_nr13[] = {
  SFX_NEXT_ROUND_DURATION, 0xA2,
  SFX_NEXT_ROUND_DURATION, 0x82,
  SOUND_END
};

const uint8_t sfx_next_round_nr14[] = {
  SFX_NEXT_ROUND_DURATION, 0x87,
  SFX_NEXT_ROUND_DURATION, 0x87,
  SOUND_END
};

void sfx_next_round(void) {
  NR10_REG = 0;
  NR11_REG = 0b01000000 | 0;
  NR12_REG = envelope(7, 0, 6);
  register_init(&nr13, sfx_next_round_nr13);
  register_init(&nr14, sfx_next_round_nr14);
}

#define SFX_TRANSPORTER 4

const uint8_t sfx_transporter_nr13[] = {
  SFX_TRANSPORTER, 0x6B,
  SFX_TRANSPORTER, 0x39,
  SFX_TRANSPORTER, 0x6B,
  SFX_TRANSPORTER, 0x73,
  SFX_TRANSPORTER, 0x6B,
  SFX_TRANSPORTER, 0x39,
  SFX_TRANSPORTER, 0x6B,
  SFX_TRANSPORTER, 0x73,
  SOUND_END
};

const uint8_t sfx_transporter_nr14[] = {
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SFX_TRANSPORTER, 0xC7,
  SOUND_END
};

void sfx_no_no_square(void) {
  NR10_REG = 0;
  NR11_REG = 0b01000000 | 0;
  NR12_REG = envelope(7, 0, 6);
  register_init(&nr13, sfx_transporter_nr13);
  register_init(&nr14, sfx_transporter_nr14);
}

uint8_t sfx_melee_attack_nr42[] = {
  20, envelope(1, 1, 2),
  10, envelope(10, 0, 1),
  SOUND_END
};

uint8_t sfx_melee_attack_nr43[] = {
  20, noise_freq(3, 0, 1),
  10, noise_freq(5, 0, 1),
  SOUND_END
};

uint8_t sfx_melee_attack_nr44[] = {
  20, 0x80,
  10, 0x80,
  SOUND_END
};

uint8_t sfx_melee_attack_nr10[] = {
  40, 0,
  30, sweep(1, 0, 7),
  SOUND_END
};

uint8_t sfx_melee_attack_nr11[] = {
  40, 0,
  30, 0,
  SOUND_END
};

uint8_t sfx_melee_attack_nr12[] = {
  40, 0,
  30, envelope(15, 0, 7),
  SOUND_END
};

uint8_t sfx_melee_attack_nr13[] = {
  40, 0,
  30, 0xFF,
  SOUND_END
};

uint8_t sfx_melee_attack_nr14[] = {
  40, 0x02,
  30, 0x82,
  SOUND_END
};

void sfx_melee_attack(void) {
  NR41_REG = 63;
  register_init(&nr42, sfx_melee_attack_nr42);
  register_init(&nr43, sfx_melee_attack_nr43);
  register_init(&nr44, sfx_melee_attack_nr44);

  register_init(&nr10, sfx_melee_attack_nr10);
  register_init(&nr11, sfx_melee_attack_nr11);
  register_init(&nr12, sfx_melee_attack_nr12);
  register_init(&nr13, sfx_melee_attack_nr13);
  register_init(&nr14, sfx_melee_attack_nr14);
}


void sfx_test(void) {

  sfx_melee_attack();

  // "Hard Fall"
  // NR10_REG = sweep(7, 1, 7);
  // NR11_REG = 0;
  // NR12_REG = envelope(15, 0, 7);
  // NR13_REG = 0x68;
  // NR14_REG = 0x85;

  // "Brrrrunnn" Sound
  // NR10_REG = sweep(5, 1, 7);
  // NR11_REG = 0;
  // NR12_REG = envelope(15, 0, 7);
  // NR13_REG = 0xFF;
  // NR14_REG = 0x84;

  // "Zip Up" Sound
  // NR10_REG = sweep(2, 0, 7);
  // NR11_REG = 0;
  // NR12_REG = envelope(15, 0, 7);
  // NR13_REG = 0xFF;
  // NR14_REG = 0x80;



  // "Rip" Sound
  // NR10_REG = sweep(1, 0, 7);
  // NR11_REG = 0;
  // NR12_REG = envelope(15, 0, 7);
  // NR13_REG = 0xFF;
  // NR14_REG = 0x82;

}

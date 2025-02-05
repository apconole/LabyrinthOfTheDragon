#pragma bank 8

#include "floor.h"

//------------------------------------------------------------------------------
// Floorwide settings
//------------------------------------------------------------------------------

#define ID 99
#define DEFAULT_X 14
#define DEFAULT_Y 14

//------------------------------------------------------------------------------
// Maps
//------------------------------------------------------------------------------

static const Map maps[] = {
  // id, bank, data, width, height
  { MAP_A, BANK_16, floor_five_data, 32, 32 },
  { MAP_B, BANK_16, floor_five_sub_data, 15, 10 },

  { END },
};

//------------------------------------------------------------------------------
// Chests
//------------------------------------------------------------------------------

static const Chest chests[] = {
  /*
  {
    CHEST_1,    // Always use a CHEST_* here cause it acts like a flag
    MAP_A,      // Map where the chest resides
    0, 0,       // (x, y) location for the chest
    false,      // Whether or not the chest is locked
    false,      // If locked, can it be opened using a magic key?
    "Nice!",    // Message to display when the chest is opened
    NULL,       // Item list contents (optional)
    NULL,       // Scripting "on open" callback (optional)
  }
  */
  { CHEST_1, MAP_A,  2,  2, false, false, NULL, NULL, chest_add_magic_key },
  { CHEST_1, MAP_A,  9, 10, false, false, NULL, NULL, chest_add_magic_key },
  { CHEST_1, MAP_A,  6, 16, false, false, str_chest_item_2pot_1eth, chest_item_2pot_1eth },
  { CHEST_1, MAP_A, 21, 18, false, false, NULL, NULL, chest_add_magic_key },
  { CHEST_1, MAP_A, 23, 11, false, false, NULL, NULL, chest_add_magic_key },

  { END },
};

//------------------------------------------------------------------------------
// Exits
//------------------------------------------------------------------------------

static const Exit exits[] = {
  /*
  {
    MAP_A,        // Map the exit is on
    0, 0,         // Column and row on that map for the exit
    FLOOR_TEST_ID,    // Floor to which the exit leads (last door, basically)
    DEST_MAP      // Id of the destination map
    0, 0,         // Column and row
    UP,           // Way the player should be facing leaving the exit
    EXIT_STAIRS   // Type of exit (not sure if we'll use this yet)
  },
  */
  // Boss
  { MAP_A, 3, 18, MAP_B, 3, 8, UP, EXIT_STAIRS },
  { MAP_B, 3, 8, MAP_A, 3, 18, DOWN, EXIT_STAIRS },

  // Elite
  { MAP_A, 26, 6, MAP_B, 12, 4, UP, EXIT_STAIRS },
  { MAP_B, 12, 4, MAP_A, 26, 6, DOWN, EXIT_STAIRS },

  { MAP_B, 3, 2, MAP_A, 3, 2, DOWN, EXIT_STAIRS, &bank_floor5 },
  { END },
};

//------------------------------------------------------------------------------
// Exits
//------------------------------------------------------------------------------

static const Sign signs[] = {
  /*
  {
    MAP_A,      // Id of the map
    0, 0,       // Position in the map for the sign
    UP,         // Direction the player must be facing
    "Hi there!" // The message to display
  }
  */
  { MAP_A,  2, 18, UP, str_floor_common_tbd }, // Boss
  { MAP_A, 13, 14, UP, str_floor_common_tbd }, // Entrance
  { MAP_A, 25,  6, UP, str_floor_common_tbd }, // Elite

  { END },
};

//------------------------------------------------------------------------------
// Levers
//------------------------------------------------------------------------------

static const Lever levers[] = {
  /*
  {
    LEVER_1,  // Use the LEVER_* constants for ids (again, used as flags)
    MAP_A,    // The map where the lever be
    0, 0,     // (x, y) tile coordinates in the map
    false,    // Can the lever only be pulled once?
    false,    // Does the lever start stuck? (requires scripting to change)
    NULL,     // Scripting callback for the lever
  }
  */
  { END },
};

//------------------------------------------------------------------------------
// Doors
//------------------------------------------------------------------------------

static const Door doors[] = {
  /*
  {
    DOOR_1,           // Use DOOR_* constants for ids.
    MAP_A,            // Map for the door
    0, 0              // (x, y) tile for the door
    DOOR_STAIRS_UP    // Kind of door
    true              // Magic key required to unlock
    false,            // Does the door start opened?
  }
  */
  { DOOR_1, MAP_B,  3, 2, DOOR_NEXT_LEVEL, false },

  { END }
};

//------------------------------------------------------------------------------
// Sconces
//------------------------------------------------------------------------------

static const Sconce sconces[] = {
  /*
  {
    SCONCE_1,   // Use SCONCE_* constants for ids.
    MAP_A,      // Map for the sconce
    0, 0,       // (x, y) tile for the sconce
    true,       // Should the sconce start lit
    FLAME_BLUE  // Flame color for the sconce if it starts lit.
  }
  */
  // BOSS
  { SCONCE_STATIC, MAP_B, 2, 2, true, FLAME_RED },
  { SCONCE_STATIC, MAP_B, 4, 2, true, FLAME_RED },

  // ELITE
  { SCONCE_STATIC, MAP_B, 11,  1, true, FLAME_RED },
  { SCONCE_STATIC, MAP_B, 13, 1, true, FLAME_RED },

  // MAZE
  { SCONCE_STATIC, MAP_A, 7, 3, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 11, 4, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 27, 6, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 30, 7, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 28, 11, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 15, 14, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 23, 14, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 4, 18, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 12, 20, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 18, 20, true, FLAME_RED },
  { SCONCE_STATIC, MAP_A, 8, 23, true, FLAME_GREEN },
  { SCONCE_STATIC, MAP_A, 4, 26, true, FLAME_RED },
  // { SCONCE_STATIC, MAP_A, 14, 26, true, FLAME_RED },
  // { SCONCE_STATIC, MAP_A, 29, 27, true, FLAME_RED },

  { END }
};

//------------------------------------------------------------------------------
// NPCs (IMPLS YET)
//------------------------------------------------------------------------------

static void on_boss_victory(void) NONBANKED {
  open_door(DOOR_1);
  set_npc_invisible(NPC_1);
}

static void on_elite_victory(void) NONBANKED {
  // TODO: correct ability given
  grant_ability(ABILITY_1);
  set_npc_invisible(NPC_2);
  map_textbox(str_floor_common_new_ability);
}

static bool on_boss_encouter(void) {
  Monster *monster = encounter.monsters;
  reset_encounter(MONSTER_LAYOUT_1);
  kobold_generator(monster, player.level, B_TIER);
  monster->id = 'A';
  set_on_victory(on_boss_victory);
  start_battle();
  return true;
}

static bool on_elite_encouter(void) {
  Monster *monster = encounter.monsters;
  reset_encounter(MONSTER_LAYOUT_1);
  kobold_generator(monster, player.level, B_TIER);
  monster->id = 'A';
  set_on_victory(on_elite_victory);
  start_battle();
  return true;
}

static bool on_npc_action(const NPC *npc) {
  if (npc->id == NPC_1){
    map_textbox_with_action(str_floor_common_fight_me, on_boss_encouter);
  } else {
    map_textbox_with_action(str_floor_common_love, on_elite_encouter);
  }
  return true;
}

static const NPC npcs[] = {
  /*
  {
    NPC_1,            // Use NPC_* constants for ids.
    MAP_A,            // Map for the npc
    0, 0              // (x, y) tile for the npc
    MONSTER_KOBOLD,   // Monster graphic to use for the NPC
    B_TIER,           // Tier for the monster (determines palette)
    action_callback,  // Action callback to execute when the player interacts
  }
  */
  { NPC_1, MAP_B, 3, 5, MONSTER_KOBOLD, S_TIER, on_npc_action }, // Boss
  { NPC_2, MAP_B, 12, 2, MONSTER_KOBOLD, A_TIER, on_npc_action }, // Elite

  { END }
};

//------------------------------------------------------------------------------
// Scripting Callbacks
//------------------------------------------------------------------------------

static bool on_init(void) {
  return false;
}

static bool on_special(void) {
  if (player_at(6, 27)) {
    map_textbox(str_floor_common_strange_wind);
    return true;
  }
  return false;
}

static bool on_move(void) {
  return false;
}

static bool on_action(void) {
  return false;
}

//------------------------------------------------------------------------------
// Palette Colors
//------------------------------------------------------------------------------

static const palette_color_t palettes[] = {
  // Palette 1 - Core background tiles
  RGB8(190, 200, 190),
  RGB8(100, 100, 140),
  RGB8(40, 60, 40),
  RGB8(24, 0, 0),
  // Palette 2 - Treasure chests
  RGB8(192, 138, 40),
  RGB8(100, 100, 140),
  RGB8(40, 60, 40),
  RGB8(24, 0, 0),
  // Palette 3
  RGB_WHITE,
  RGB8(120, 120, 120),
  RGB8(60, 60, 60),
  RGB_BLACK,
  // Palette 4
  RGB_WHITE,
  RGB8(120, 120, 120),
  RGB8(60, 60, 60),
  RGB_BLACK,
  // Palette 5
  RGB_WHITE,
  RGB8(120, 120, 120),
  RGB8(60, 60, 60),
  RGB_BLACK,
  // Palette 6
  RGB_WHITE,
  RGB8(120, 120, 120),
  RGB8(60, 60, 60),
  RGB_BLACK,
  // Palette 7
  RGB_WHITE,
  RGB8(120, 120, 120),
  RGB8(60, 60, 60),
  RGB_BLACK,
};

//------------------------------------------------------------------------------
// Area Definition (shouldn't have to touch this)
//------------------------------------------------------------------------------
const Floor floor5 = {
  ID, DEFAULT_X, DEFAULT_Y, palettes,
  maps,
  exits,
  chests,
  signs,
  levers,
  doors,
  sconces,
  npcs,
  on_init,
  on_special,
  on_move,
  on_action,
};

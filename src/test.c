#include <stdint.h>
#include <stdio.h>

#include "battle.h"
#include "core.h"
#include "floor.h"
#include "item.h"
#include "map.h"
#include "sound.h"
#include "stats.h"

void init_test_player(PlayerClass c, uint8_t level) {
  init_player(c);
  grant_ability(ABILITY_ALL);
  set_player_level(level);
  sprintf(player.name, "Tester");
  player.message_speed = AUTO_PAGE_FAST;
}

void test_battle_init(void) {
  encounter.is_test = true;
  toggle_sprites();
  init_battle();
}

void fill_inventory(uint8_t amt) {
  Item *item = inventory;
  for (uint8_t k = 0; k < INVENTORY_LEN; k++, item++)
    item->quantity = amt;
}

void test_big_map(void) {
  init_test_player(CLASS_DRUID, 8);
  // grant_ability(ABILITY_ALL);
  // set_active_floor(&floor_test);
  // player.has_torch = true;
  set_active_floor(&floor_test2);
  init_world_map();
  game_state = GAME_STATE_WORLD_MAP;
}

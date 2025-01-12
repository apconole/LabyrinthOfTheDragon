#ifndef _MONSTER_SFX_H
#define _MONSTER_SFX_H

#include "battle.h"
#include "sound.h"
#include "strings.h"

/**
 * Plays the "miss" sound during battle.
 */
#define SFX_MISS battle_sfx = sfx_miss

/**
 * Plays the "monster melee hit" sound during battle.
 */
#define SFX_MELEE battle_sfx = sfx_monster_attack1

/**
 * Plays the "monster magic hit" sound during battle.
 */
#define SFX_MAGIC battle_sfx = sfx_monster_attack2

/**
 * Plays the "action failed" sound during battle.
 */
#define SFX_FAIL battle_sfx = sfx_monster_fail

/**
 * Plays the "critical hit" sound during battle.
 */
#define SFX_CRIT battle_sfx = sfx_monster_critical

/**
 * Plays the "healing" sound during battle.
 */
#define SFX_HEAL battle_sfx = sfx_heal

/**
 * Plays the "fleeing" sound during battle.
 */
#define SFX_FLEE battle_sfx = sfx_stairs

/**
 * Sets the flag to skip the post action message in the battle system.
 */
#define SKIP_POST_MSG skip_post_message = true

/**
 * Sets the message and sound effect for when a player misses with an attack.
 */
#define PLAYER_MISS do { \
  sprintf(battle_post_message, str_player_miss); \
  SFX_MISS; \
  } while(0)

/**
 * Sets the message and sound effect for when a player misses all monsters with
 * a multi-target attack.
 */
#define PLAYER_MISS_ALL do { \
  sprintf(battle_post_message, str_player_miss_all); \
  SFX_MISS; \
  } while(0)

/**
 * Sets the message and sound effect for when the player is healed.
 */
#define PLAYER_HEAL(hp) do { \
  sprintf(battle_post_message, str_player_heal_hp, (hp)); \
  SFX_HEAL; \
  } while (0)

/**
 * Sets state, message, and sound effects when a monster flees.
 */
#define MONSTER_FLEE(monster) do { \
  sprintf(battle_post_message, str_monster_flee_success); \
  monster->fled = true; \
  SFX_FLEE; \
  } while (0)

/**
 * Sets state, message, and sound effects when a monster fails to flee.
 */
#define MONSTER_FLEE_FAIL(monster) do { \
  sprintf(battle_post_message, str_monster_flee_failure); \
  monster->fled = false; \
  SFX_FAIL; \
  } while (0)

#endif

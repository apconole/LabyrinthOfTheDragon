/*
 * strings.js
 *
 * Defines and exports a series of namespaced strings meant to be preformatted
 * and transpiled into C files for inclusion into the ROM.
 */

/**
 * Namespaces to export.
 */
const namespaces = {};

/**
 * Adds a string namespace. Each namespace generates a separate banked C file
 * that defines the formatted string constants.
 * @param {string} name Name for the namespace.
 * @param {number} bank ROM bank in which to write the strings.
 * @param {object} strings Key value pairs of names and string values.
 */
function addNamespace(name, bank, strings) {
  namespaces[name] = { bank, strings };
}

addNamespace('misc', 0, {
  'empty': 'EMPTY...',
  'physical': '',
  'magical': 'magical',
  'earth': 'earth',
  'water': 'water',
  'air': 'air',
  'fire': 'fire',
  'light': 'light',
  'dark': 'dark',
  'potion':        'Potion',
  'remedy':        'Remedy',
  'ether':         'Ether ',
  'atk_up_potion': "ATK ^ ",
  'def_up_potion': 'DEF ^ ',
  'elixer':        'Elixer',
  'regen_pot':     'Regen ',
  'haste_pot':     'Haste ',
  'dummy': 'Dummy',
  'kobold': 'Kobold',
  'goblin': 'Goblin',
  'zombie': 'Zombie',
  'bugbear': 'Bugbear',
  'owlbear': 'Owlbear',
  'gelatinous_cube': 'G.Cube',
  'displacer_beast': 'D.Beast',
  'will_o_wisp': 'W.O.Wisp',
  'death_knight': 'D.Knight',
  'mind_flayer': 'M.Flayer',
  'beholder': 'Beholder',
  'dragon': 'Dragon',
})

addNamespace('ability', 0, {
  'druid_poison_spray':  'Poison Spray ',
  'druid_cure_wounds':   'Cure Wounds  ',
  'druid_bark_skin':     'Bark Skin    ',
  'druid_ligtning':      'Lightning    ',
  'druid_heal':          'Heal         ',
  'druid_insect_plague': 'Insect Plague',
  'druid_regen':         'Regenerate   ',
});

addNamespace('battle', 0, {
  'poison_spray': 'Poison gas erupts from your palm!',
  'cure_wounds': "You're enveloped in blue light...",
  'bark_skin': 'Your skin grows hard as wood.',
  'lightning': 'Bolts of lighting fall!',
  'heal': 'Radiant green light descends...',
  'heal_complete': "You're fully healed!",
  'insect_plague': 'Locusts swarm!',
  'regen': 'You surge with vitality!',
  'monster_scared_frozen': '%monster %c shivers in fear...',
  'monster_paralyzed': "%monster %c can't move!",
  'monster_poison_death': '%monster %c succumbs to the poison!',
  'monster_confuse_attack_self': 'Confused, %monster %c attacks itself!',
  'monster_confuse_attack_other': 'Confused, %monster %c attacks any ally!',
  'monster_confuse_stupor': '%monster %c stares aimlessly.',
  'player_miss': 'But you miss!',
  'player_magic_miss': 'But it has no effect!',
  'player_hit': 'You deal %damage damage!',
  'player_hit_immune': "They're completely immune!",
  'player_hit_resist': "They resist your attack, only %damage damage...",
  'player_hit_vuln': "SUPER EFFECTIVE %damage damage!",
  'player_hit_crit': 'CRITICAL HIT! You deal %damage damage!',
  'player_heal': 'You heal %damage HP.',
  'player_heal_crit': 'CRITICAL! You heal a whopping %damage HP!',
  'player_heal_fumble': 'You only heal a measly %damage HP.',
  'player_miss_all': 'A COMPLETE WHIFF.',
  'player_flee_attempt': 'You attempt to flee...',
  'player_flee_success': 'And get away!',
  'player_flee_failure': 'But are blocked!',
  'victory': 'Victory! You gain %exp XP!',
  'victory_no_xp': 'Victory! But you gain no XP...',
  'level_up': 'LEVEL UP! You reach %level!',
})

addNamespace('maps', 2, {
  'chest_locked': "The chest is locked.",
  'chest_open': "You opened the chest!",
  "chest_empty": "The chest was empty.",
  'chest_key_locked': "You need a magic key to unlock this chest...",
  'chest_unlock_key': "You unlock the chest with a magic key!",
  'get_magic_key': "You get a magic key!",
  'get_torch': "You find a torch!",
  'lever_stuck': "It's stuck!",
  'lever_one_way': "Seems this lever was one and done.",
  'door_locked': 'The door is locked.',
  'door_locked_key': 'You need a magic key to unlock this door...',
  'door_unlock_key': 'You unlock the door with a magic key!',
  'sconce_lit_no_torch': 'The sconce burns brightly.',
  'sconce_no_torch': 'Hmm... how do you light this?',
  'sconce_torch_not_lit': 'Your torch lacks a flame.',
});

addNamespace('chest_item', 2, {
  '2pot_1eth': 'You get 2 potions and an ether!',
  'haste_pot': 'You get a haste potion!',
  'regen_pot': 'You get a regen potion!',
});

addNamespace('items', 3, {
  'use_potion': '%damage HP healed!',
  'use_ether_sp': '%damage SP restored!',
  'use_ether_mp': '%damage MP restored!',
  'use_remedy': 'You remedy what ails you!',
  'use_atkup': 'Your attack increases!',
  'use_defup': 'Your defense increases!',
  'use_elixer': 'You fully heal!',
  'use_regen': 'You begin regenerating!',
  'use_haste': 'The world slows down!',
  'use_failed': "The item didn't work!",
})

addNamespace('monster', 6, {
  'flee': '%monster %c makes a run for it...',
  'flee_failure': 'But they cannot get away!',
  'flee_success': 'And they get away!',
  'attack': '%monster %c attacks!',
  'miss': 'But they miss!',
  'magic_miss': 'But it has no effect!',
  'hit': 'You take %damage damage!',
  'hit_aspect': 'You take %damage %aspect damage!',
  'hit_immune': "But you're completely immune!",
  'hit_resist': 'You resist, only %damage damage',
  'hit_vuln': "It's SUPER BAD! %damage damage!",
  'hit_crit': 'CRITICAL HIT! You take %damage damage!',
  'hit_barkskin': 'Your barkskin protects you! %damage damage.',
  'does_nothing': '%monster %c does nothing.',
  'dummy_pre': 'Dummy %c stands still.',
  'dummy_post_heal': '"I will never die..."',
  'kobold_axe': 'Kobold %c raises a tiny axe...',
  'kobold_fire': 'Kobold %c spits a glob of fire...',
  'kobold_dazed': 'Kobold %c looks dazed...',
  'kobold_does_nothing': 'And does nothing!',
  'kobold_miss': 'But instead it falls over and hiccups!',
  'kobold_get_up': 'Kobold %c gets up.',
  'goblin_nose_pick': 'Goblin %c picks its nose.',
  'goblin_attack': "Goblin %c swings a shortsword!",
  'goblin_acid_arrow': "Goblin %c shoots an acid arrow!",
  'zombie_brains': 'Hrrnng... brains.',
  'zombie_bite_miss': "Zombie %c's bite barely misses!",
  'zombie_bite_hit': "Zombie %c bites and poisons you!",
  'zombie_slam': "Zombie %c swipes at you!",
})

// Export the namespaces
module.exports = namespaces;

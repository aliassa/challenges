#ifndef SG_COMBAT_H
#define SG_COMBAT_H

#include "game_state.h"
#include "entities/ship.h"
#include "entities/weapon.h"

AttackRes ship_fire_weapon(GameState* gs, Ship* attacker, Ship* target, Weapon* weapon);

#endif // SG_COMBAT_H


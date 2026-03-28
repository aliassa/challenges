#include "combat.h"
#include "shared/utils.h"

static int random_hit(float accuracy) {
    return random_float() <= accuracy ? 1 : 0;
}

AttackRes ship_fire_weapon(GameState* gs, Ship* attacker, Ship* target, Weapon* weapon) {
    if(!attacker || !target || !weapon || !gs) return ATTACK_INVALID_INPUT;
    if(weapon->count == 0) return ATTACK_OUT_OF_MUNITION;
    if(!ship_has_cargo(attacker, weapon, CARGO_WEAPON)) return ATTACK_INVALID_INPUT;
    if(distance(attacker->position, target->position) > weapon->range) return ATTACK_OUT_OF_RANGE;
    
    weapon->count--;
    
    if(random_hit(weapon->accuracy)) {
        if(target->health <= weapon->damage) {
            ship_destroy(gs->ships, target);
            return ATTACK_DESTROYED;
        }
        target->health -= weapon->damage;
    }
    return ATTACK_DAMAGED;
}
#include "combat.h"
#include "shared/utils.h"
#include <stdio.h>

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
    else return ATTACK_MISSED;
    return ATTACK_DAMAGED;
}

void print_attack_result(AttackRes res) {
    switch (res)
    {
    case ATTACK_INVALID_INPUT:
        printf("Attack : %s\n", "ATTACK_INVALID_INPUT");
        break;
    case ATTACK_OUT_OF_MUNITION:
        printf("Attack : %s\n", "ATTACK_OUT_OF_MUNITION");
        break;
    case ATTACK_OUT_OF_RANGE:
        printf("Attack : %s\n", "ATTACK_OUT_OF_RANGE");
        break;
    case ATTACK_DESTROYED:
        printf("Attack : %s\n", "ATTACK_DESTROYED");
        break;
    case ATTACK_DAMAGED:
        printf("Attack : %s\n", "ATTACK_DAMAGED");
        break;
    case ATTACK_MISSED:
        printf("Attack : %s\n", "ATTACK_MISSED");
        break;
    default:
        printf("Attack : UNSUPPORTED ATTACK_RES\n");
        break;
    }
}
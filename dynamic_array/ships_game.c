#include "shared/dynamic_array.h"

#include "shared/types.h"
#include "shared/utils.h"

#include "entities/ship.h"
#include "entities/cargo_item.h"
#include "entities/soldier.h"
#include "entities/weapon.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

size_t g_id;

#define PI 3.14f


static const char* type_to_str(CargoType type) {
    switch (type)
    {
        case CARGO_SOLDIER: return "SOLDIER"; break;
        case CARGO_STAFF: return "STAFF"; break;
        case CARGO_WEAPON: return "WEAPON"; break;
        case CARGO_FOOD: return "FOOD"; break;
        default: return "UNKNOWN"; break;
    }
}

typedef struct {
    void* p; // Soldier*, Weapon*, Food*, ....
    CargoType type;
    float weight;
} CargoItem;

static void print_cargo(void* s) {
    if(!s) return;
    printf("Cargo\n");
    printf("Weight : %.2f Kg\n", ((CargoItem*)s)->weight );
    printf("Type : %s\n", type_to_str(((CargoItem*)s)->type));
    switch (((CargoItem*)s)->type)
    {
    case CARGO_SOLDIER:
        print_soldier(((CargoItem*)s)->p);
        break;
    case CARGO_WEAPON:
        print_weapon(((CargoItem*)s)->p);
    default:
        break;
    }
}

Soldier* soldier_create(vec_t* soldiers, const char* name, Rank rank, float weight) {
    Soldier* s = calloc(1, sizeof *s);
    if(!s) return NULL;
    snprintf(s->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");
    s->rank = rank;

    s->id = g_id++;
    s->weight = weight;
    vec_push(soldiers, s);
    return s;
}


int ship_cargo_add(Ship* ship, CargoType type, void* obj, float weight) {
    if (!ship) return -1;

    if(ship->total_weight + weight > ship->max_weight)  return -2;
    if(ship->nb_onboard  == ship->max_onboard)  return -3;
    CargoItem* item = malloc(sizeof *item);
    if(!item) return -4;
    
    ship->total_weight += weight;
    item->type = type;
    item->p = obj;
    item->weight = weight;

    if(type == CARGO_SOLDIER || type == CARGO_STAFF) ship->nb_onboard++;

    vec_push(ship->cargo[type], item);

    return 0;
}

int ship_has_cargo(Ship* s, void* cargo, CargoType type) {
    // Check if a ship has a specefic cargo
    if(!s || !cargo) return 0;
    assert(type >= 0 && type < CARGO_COUNT);
    vec_t* v = s->cargo[type];
    for(size_t i = 0; i < v->length; i++) {
        CargoItem* item = v->data[i];
        if(item->p == cargo) return 1;
    }
    return 0;
}

int random_hit(float accuracy) {
    return random_float() <= accuracy ? 1 : 0;
}

AttackRes ship_fire_weapon(vec_t* ships, Ship* attacker, Ship* target, Weapon* weapon) {
    if(!attacker || !target || !weapon || !ships) return ATTACK_INVALID_INPUT;
    if(weapon->count == 0) return ATTACK_OUT_OF_MUNITION;
    if(!ship_has_cargo(attacker, weapon, CARGO_WEAPON)) return ATTACK_INVALID_INPUT;
    if(distance(attacker->position, target->position) > weapon->range) return ATTACK_OUT_OF_RANGE;
    
    weapon->count--;
    
    if(random_hit(weapon->accuracy)) {
        if(target->health <= weapon->damage) {
            ship_destroy(ships, target);
            return ATTACK_DESTROYED;
        }
        target->health -= weapon->damage;
    }
    return ATTACK_DAMAGED;
}

int ship_cargo_remove(Ship* ship, CargoType type, void* obj) {
    assert(type >= 0 || type < CARGO_COUNT);
    if (!ship || !ship->cargo[type]) return -1;

    vec_t* v = ship->cargo[type];

    for(size_t i = 0; i < v->length; i++) {
        CargoItem* item = v->data[i];
        if(item->p == obj) {
            ship->total_weight -= item->weight;

            if(item->type == CARGO_SOLDIER || item->type == CARGO_STAFF)
                ship->nb_onboard--;

            free(item);

            memmove(&v->data[i], &v->data[i+1], (v->length -i - 1) * sizeof (void*));
            v->length--;
            v->data[v->length] = NULL;

            return 0;
        }
    }
    return -2;
}

int main(void) {
    // Seed for random gen
    srand(time(NULL));

    vec_t* soldiers = vec_create(10);
    vec_t* ships = vec_create(10);
    Soldier* s1 = soldier_create(soldiers, "Ali BOUZIDI", RANK_ADMIRAL, 78.5f);
    Soldier* s2 = soldier_create(soldiers, "Ilyes BOUZIDI", RANK_CAPTAIN, 14.5f);
    Soldier* s3 = soldier_create(soldiers, "Ines TISS", RANK_SEAMN, 68.f);

    Ship* sh1 = ship_create(ships, "OMAR IBN KHATTAB", 1024, 10000000.f, 10.f, 100.f, 100000.f);

    Weapon* w1 = weapon_create("Kruz", 8, 50, 0.8f, 100, 1000.f);

    ship_cargo_add(sh1, CARGO_SOLDIER, s1, s1->weight);
    ship_cargo_add(sh1, CARGO_SOLDIER, s2, s2->weight);
    ship_cargo_add(sh1, CARGO_SOLDIER, s3, s3->weight);
    ship_cargo_add(sh1, CARGO_WEAPON, w1, w1->weight);
    

    vec_print(sh1->cargo[CARGO_SOLDIER], print_cargo);
    vec_print(sh1->cargo[CARGO_WEAPON], print_cargo);
    // ____________________ Clean up ________________________________
    free(s1); s1 = NULL;
    free(s2); s2 = NULL;
    free(s3); s3 = NULL;

    free(sh1); sh1 = NULL;

    vec_destroy(soldiers);
    vec_destroy(ships);
}


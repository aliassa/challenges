#include "shared/dynamic_array.h"

#include "shared/types.h"
#include "shared/utils.h"
#include "debug.h"

#include "game_state.h"

#include "entities/ship.h"
#include "entities/cargo_item.h"
#include "entities/soldier.h"
#include "entities/weapon.h"

#include <time.h>

size_t g_id;

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


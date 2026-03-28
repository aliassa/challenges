// ships_game.c
#include "game_state.h"
#include "game_loop.h"
#include "entities/ship.h"
#include "entities/soldier.h"
#include "entities/weapon.h"
#include <stdlib.h>
#include <time.h>

size_t g_id;

int main(void) {
    srand(time(NULL));

    GameState gs = {
        .ships    = vec_create(10),
        .soldiers = vec_create(10),
        .turn     = 0,
    };

    // bootstrap initial entities
    Ship* player1 = ship_create(&gs, "OMAR IBN KHATTAB", 1024, 10000000.f, 10.f, 100.f, 100000.f);
    Ship* player2 = ship_create(&gs, "AL BURAQ", 1024, 10000000.f, 10.f, 100.f, 100000.f);
    Weapon* w1 = weapon_create("Tomahawk", 10, 100, 0.9f, 100.f, 100.f);
    Weapon* w2 = weapon_create("M5 Missile", 5, 10, 0.8f, 100.f, 10.f);
    ship_cargo_add(player1, CARGO_WEAPON, w1, w1->weight);
    ship_cargo_add(player2, CARGO_WEAPON, w2, w2->weight);
    
    ship_move(player1, 80.f, PI/2);

    game_loop_run(&gs);

    // cleanup
    // game_state_free(&gs); // TODO
    ship_destroy(gs.ships, player1);
    ship_destroy(gs.ships, player2);
    
    return 0;
}

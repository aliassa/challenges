#include "test_utils.h"
#include "../entities/ship.h"
#include "../entities/soldier.h"
#include "../entities/weapon.h"
#include "../game_state.h"
#include "../shared/dynamic_array.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

// ─── Helpers ────────────────────────────────────────────────────

size_t g_id;

static GameState make_gs(void) {
    GameState gs = {
        .ships    = vec_create(10),
        .soldiers = vec_create(10),
        .turn     = 0,
    };
    return gs;
}

static void free_gs(GameState* gs) {
    vec_destroy(gs->ships);
    vec_destroy(gs->soldiers);
}

// ─── Soldier tests ──────────────────────────────────────────────

static void test_soldier_create(void) {
    printf("\nsoldier_create\n");
    GameState gs = make_gs();

    Soldier* s = soldier_create(&gs, "Ali", RANK_ADMIRAL, 78.5f);
    ASSERT_NOTNULL(s,                        "returns non-null");
    ASSERT_STREQ(s->name, "Ali",             "name is set");
    ASSERT_EQ(s->rank, RANK_ADMIRAL,         "rank is set");
    ASSERT_EQ(gs.soldiers->length, 1,        "added to gs.soldiers");

    Soldier* s2 = soldier_create(&gs, NULL, RANK_SEAMN, 60.f);
    ASSERT_NOTNULL(s2,                       "null name returns non-null");
    ASSERT_STREQ(s2->name, "Unknown",        "null name defaults to Unknown");

    free_gs(&gs);
    free(s);
    free(s2);
}

// ─── Weapon tests ───────────────────────────────────────────────

static void test_weapon_create(void) {
    printf("\nweapon_create\n");

    Weapon* w = weapon_create("Kruz", 8, 50, 0.8f, 100.f, 1000.f);
    ASSERT_NOTNULL(w,                        "returns non-null");
    ASSERT_STREQ(w->name, "Kruz",            "name is set");
    ASSERT_EQ(w->capacity, 8,               "capacity is set");
    ASSERT_EQ(w->damage, 50,                "damage is set");
    // ASSERT_EQ(w->count, 8,                  "count initialized to capacity");

    Weapon* w2 = weapon_create(NULL, 0, 0, 0.f, 0.f, 0.f);
    ASSERT_NOTNULL(w2,                       "null name returns non-null");
    ASSERT_STREQ(w2->name, "Unknown",        "null name defaults to Unknown");

    free(w);
    free(w2);
}

// ─── Ship tests ─────────────────────────────────────────────────

static void test_ship_create(void) {
    printf("\nship_create\n");
    GameState gs = make_gs();

    Ship* s = ship_create(&gs, "Titanic", 100, 5000.f, 10.f, 10.f, 1000.f);
    ASSERT_NOTNULL(s,                        "returns non-null");
    ASSERT_STREQ(s->name, "Titanic",         "name is set");
    ASSERT_EQ(s->max_onboard, 100,           "max_onboard is set");
    ASSERT_EQ(s->health, SHIP_HEALTH_MAX,    "health starts at max");
    ASSERT_EQ(s->fuel, s->max_fuel,          "fuel starts full");
    ASSERT_EQ(gs.ships->length, 1,           "added to gs.ships");

    free_gs(&gs);
    free(s);
}

static void test_ship_refuel(void) {
    printf("\nship_refuel\n");
    GameState gs = make_gs();
    Ship* s = ship_create(&gs, "X", 10, 1000.f, 5.f, 10.f, 100.f);

    s->fuel = 0.f;
    ship_refuel(s, 50.f);
    ASSERT_EQ(s->fuel, 50.f,                 "partial refuel works");

    ship_refuel(s, 9999.f);
    ASSERT_EQ(s->fuel, s->max_fuel,          "overfill clamps to max_fuel");

    ASSERT_EQ(ship_refuel(NULL, 10.f), -1,   "null ship returns -1");

    free_gs(&gs);
    free(s);
}

static void test_ship_move(void) {
    printf("\nship_move\n");
    GameState gs = make_gs();
    Ship* s = ship_create(&gs, "X", 10, 1000.f, 5.f, 1.f, 100.f);

    // move right (direction = 0 rads)
    float moved = ship_move(s, 30.f, 0.f);
    ASSERT_EQ(moved, 30.f,                   "returns distance moved");
    ASSERT_EQ(s->fuel, 70.f,                 "fuel decremented correctly");

    // not enough fuel — should clamp
    s->fuel = 10.f;
    moved = ship_move(s, 999.f, 0.f);
    ASSERT_EQ(moved, 10.f,                   "clamps to remaining range");
    ASSERT_EQ(s->fuel, 0.f,                  "fuel reaches zero");

    ASSERT_EQ(ship_move(NULL, 10.f, 0.f), -1.f, "null ship returns -1");

    free_gs(&gs);
    free(s);
}

static void test_ship_cargo_add_remove(void) {
    printf("\nship_cargo_add / ship_cargo_remove\n");
    GameState gs = make_gs();
    Ship*    sh = ship_create(&gs, "X", 10, 1000.f, 5.f, 1.f, 1000.f);
    Soldier* s  = soldier_create(&gs, "Ali", RANK_SEAMN, 70.f);
    Weapon*  w  = weapon_create("Gun", 5, 10, 0.9f, 50.f, 500.f);

    ASSERT_EQ(ship_cargo_add(sh, CARGO_SOLDIER, s, s->weight), 0,  "add soldier ok");
    ASSERT_EQ(sh->nb_onboard, 1,                                    "nb_onboard incremented");
    ASSERT_EQ(sh->total_weight, s->weight,                          "total_weight updated");
    ASSERT_EQ(ship_has_cargo(sh, s, CARGO_SOLDIER), 1,              "has_cargo finds soldier");

    ASSERT_EQ(ship_cargo_add(sh, CARGO_WEAPON, w, w->weight), 0,   "add weapon ok");
    ASSERT_EQ(ship_has_cargo(sh, w, CARGO_WEAPON), 1,               "has_cargo finds weapon");

    // exceed max weight
    ASSERT_EQ(ship_cargo_add(sh, CARGO_SOLDIER, s, 99999.f), -2,   "rejects overweight");

    // exceed max onboard
    sh->nb_onboard = sh->max_onboard;
    ASSERT_EQ(ship_cargo_add(sh, CARGO_SOLDIER, s, 1.f), -3,       "rejects over capacity");
    sh->nb_onboard = 1;

    ASSERT_EQ(ship_cargo_remove(sh, CARGO_SOLDIER, s), 0,           "remove soldier ok");
    ASSERT_EQ(sh->nb_onboard, 0,                                    "nb_onboard decremented");
    ASSERT_EQ(ship_has_cargo(sh, s, CARGO_SOLDIER), 0,              "has_cargo no longer finds it");

    ASSERT_EQ(ship_cargo_remove(sh, CARGO_SOLDIER, s), -2,          "remove missing returns -2");

    free_gs(&gs);
    free(s);
    free(w);
    free(sh);
}

static void test_ship_get_max_range(void) {
    printf("\nship_get_max_range\n");
    GameState gs = make_gs();
    Ship* s = ship_create(&gs, "X", 10, 1000.f, 5.f, 2.f, 100.f);

    ASSERT_EQ(ship_get_max_range(s), 50.f,   "max range = fuel / consumption");

    free_gs(&gs);
    free(s);
}

// ─── Entry point ────────────────────────────────────────────────

int main(void) {
    srand(time(NULL));
    printf("=== Entity tests ===");

    test_soldier_create();
    test_weapon_create();
    test_ship_create();
    test_ship_refuel();
    test_ship_move();
    test_ship_cargo_add_remove();
    test_ship_get_max_range();

    TEST_SUMMARY();
}
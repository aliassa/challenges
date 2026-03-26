#include "dynamic_array.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>


#define MAX_NAME_LENGTH 32

typedef enum {
    CARGO_SOLDIER,
    CARGO_STAFF,
    CARGO_MUNITION,
    CARGO_FOOG,
} CargoType;

typedef enum {
    RANK_UNKNOWN,
    RANK_SEAMN,
    RANK_PETTY_OFFICER,
    RANK_WARRANT_OFFICER,
    RANK_LIEUTENANT,
    RANK_COMMANDER,
    RANK_CAPTAIN,
    RANK_ADMIRAL,
    RANK_COUNT
} Rank;

static const char* rank_to_str(Rank rank) {
    switch (rank)
    {
        case RANK_SEAMN: return "SEAMN"; break;
        case RANK_PETTY_OFFICER: return "PETTY_OFFICER"; break;
        case RANK_CAPTAIN: return "CAPTAIN"; break;
        case RANK_WARRANT_OFFICER: return "WARRANT_OFFICER"; break;
        case RANK_LIEUTENANT: return "LIEUTENANT"; break;
        case RANK_COMMANDER: return "COMMANDER"; break;
        case RANK_ADMIRAL: return "ADMIRAL"; break;
        default: return "UNKNOWN"; break;
    }
}

typedef struct {
    float x;
    float y;
}Position;

typedef struct {
    CargoType type;
    void* p; // Soldier*, Munition*, Food*, ....
} CargoItem;

typedef struct
{
    char name[MAX_NAME_LENGTH];
    Rank rank;
    uint32_t id;
    float weight;
    uint32_t points;
    Position position;
} Soldier;

typedef struct {
    char name[MAX_NAME_LENGTH];
    uint32_t id;
    uint32_t nb_onboard;
    uint32_t max_onboard;
    float max_weight;
    float total_weight;
    Position position;

    vec_t* cargo;
} Ship;

Ship* ship_create(vec_t* ships,const char* name, uint32_t max_onboard, float max_weight) {
    Ship* s = calloc(1, sizeof *s);
    if(!s) return NULL;
    static uint32_t ship_id = 1;
    snprintf(s->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");

    s->id = ship_id++;
    s->max_onboard = max_onboard;
    s->max_weight = max_weight;
    vec_push(ships, s);
    return s;
}

static void print_soldier(void* s) {
    if(!s) return;
    printf("Soldier : %s\n", ((Soldier*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Soldier*)s)->id);
    printf("Points : %" PRIu32 "\n", ((Soldier*)s)->points);
    printf("Weight : %.2f Kg\n", ((Soldier*)s)->weight);
    printf("Rank : %s\n", rank_to_str(((Soldier*)s)->rank));
}

static void print_ship(void* s) {
    if(!s) return;
    printf("Ship : %s\n", ((Ship*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Ship*)s)->id);
    printf("MAx weight : %.2f Kg\n", ((Ship*)s)->max_weight);
    printf("Total weight : %.2f Kg\n", ((Ship*)s)->total_weight);
    printf("On board : %" PRIu32 "\n", ((Ship*)s)->nb_onboard);
}

Soldier* soldier_create(vec_t* soldiers, const char* name, Rank rank, float weight) {
    Soldier* s = calloc(1, sizeof *s);
    if(!s) return NULL;
    static uint32_t soldier_id = 1;
    snprintf(s->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");
    s->rank = rank;

    s->id = soldier_id++;
    s->weight = weight;
    vec_push(soldiers, s);
    return s;
}


int ship_cargo_add(Ship* ship, CargoType type, void* obj, float weight) {
    if (!ship) return -1;

    if(ship->total_weight + weight > ship->max_weight)  return -2;
    ship->total_weight += weight;
    CargoItem* item = malloc(sizeof *item);
    if(!item) return -3;

    item->type = type;
    item->p = obj;

    if(type == CARGO_SOLDIER || type == CARGO_STAFF) ship->nb_onboard++;

    return 0;
}

int main(void) {
    vec_t* soldiers = vec_create(10);
    vec_t* ships = vec_create(10);
    Soldier* s1 = soldier_create(soldiers, "Ali BOUZIDI", RANK_ADMIRAL, 78.5f);
    Soldier* s2 = soldier_create(soldiers, "Ilyes BOUZIDI", RANK_CAPTAIN, 14.5f);
    Soldier* s3 = soldier_create(soldiers, "Ines TISS", RANK_SEAMN, 68.f);

    Ship* sh1 = ship_create(ships, "OMAR IBN KHATTAB", 1024, 100000.f );

    ship_cargo_add(sh1, CARGO_SOLDIER, s1, s1->weight);
    ship_cargo_add(sh1, CARGO_SOLDIER, s2, s2->weight);
    ship_cargo_add(sh1, CARGO_STAFF, s3, s3->weight);

    vec_print(soldiers, print_soldier);
    vec_print(ships, print_ship);

    
    
    // ____________________ Clean up ________________________________
    free(s1);
    free(s2);
    free(s3);

    free(sh1);

    vec_destroy(soldiers);
    vec_destroy(ships);
}


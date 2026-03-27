#include "dynamic_array.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>

#define MAX_NAME_LENGTH 32

size_t g_id;

#define PI 3.14f
typedef enum {
    CARGO_SOLDIER,
    CARGO_STAFF,
    CARGO_WEAPON,
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

static const char* type_to_str(Rank type) {
    switch (type)
    {
        case CARGO_SOLDIER: return "SOLDIER"; break;
        case CARGO_STAFF: return "STAFF"; break;
        case CARGO_WEAPON: return "Weapon"; break;
        default: return "UNKNOWN"; break;
    }
}

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
} Position;


typedef struct {
    void* p; // Soldier*, Weapon*, Food*, ....
    CargoType type;
    float weight;
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

typedef struct
{
    char name[MAX_NAME_LENGTH];
    uint32_t id;
    uint32_t capacity;
    uint32_t count;
    uint32_t damage;
    float weight;
} Weapon;

typedef struct {
    char name[MAX_NAME_LENGTH];
    uint32_t id;
    uint32_t nb_onboard;
    uint32_t max_onboard;
    float radius;
    float max_weight;
    float total_weight;

    float fuel;
    float max_fuel;
    float consumption_per_km;
    Position position;
    
    vec_t* cargo; // Contains items on the chip
} Ship;

static void print_soldier(void* s) {
    if(!s) return;
    printf("Soldier : %s\n", ((Soldier*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Soldier*)s)->id);
    printf("Points : %" PRIu32 "\n", ((Soldier*)s)->points);
    printf("Weight : %.2f Kg\n", ((Soldier*)s)->weight);
    printf("Rank : %s\n", rank_to_str(((Soldier*)s)->rank));
}

static void print_weapon(void* s) {
    if(!s) return;
    printf("Weapon : %s\n", ((Weapon*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Weapon*)s)->id);
    printf("Capacity : %" PRIu32 "\n", ((Weapon*)s)->capacity);
    printf("Weight : %.2f Kg\n", ((Weapon*)s)->weight);
    printf("Damage : %" PRIu32 "\n", ((Weapon*)s)->damage);
}

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

static void print_ship(void* s) {
    if(!s) return;
    printf("Ship : %s\n", ((Ship*)s)->name);
    printf("ID : %" PRIu32 "\n", ((Ship*)s)->id);
    printf("Posision : (%.2f, %.2f)\n", ((Ship*)s)->position.x, ((Ship*)s)->position.y);
    printf("Fuel : %.2f Gallons\n", ((Ship*)s)->fuel);
    printf("Max weight : %.2f Kg\n", ((Ship*)s)->max_weight);
    printf("Total weight : %.2f Kg\n", ((Ship*)s)->total_weight);
    printf("Radius : %.2f m\n", ((Ship*)s)->radius);
    printf("On board : %" PRIu32 "\n", ((Ship*)s)->nb_onboard);
}
Weapon* weapon_create(const char* name, uint32_t capacity, uint32_t damage, float weight) {
    Weapon* w = calloc(1, sizeof(*w));
    if(!w) return NULL;

    snprintf(w->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");

    w->capacity = capacity;
    w->damage = damage;
    w->weight = weight;
    w->id = g_id++;
    return w;
}

Ship* ship_create(vec_t* ships,const char* name, uint32_t max_onboard, float max_weight, float radius,
                             float consumption_per_km, float max_fuel) {
    assert(consumption_per_km != 0);
    Ship* s = calloc(1, sizeof *s);
    if(!s) return NULL;
    snprintf(s->name, MAX_NAME_LENGTH, "%s", name ? name : "Unknown");

    s->id = g_id++;
    s->max_onboard = max_onboard;
    s->max_weight = max_weight;
    s->radius = radius;
    s->max_fuel = max_fuel;
    s->fuel = max_fuel;
    s->consumption_per_km = consumption_per_km;
    s->cargo = vec_create(10);
    vec_push(ships, s);
    return s;
}

float ship_get_max_range(const Ship* s) {
    return s->fuel / s->consumption_per_km;
}

float ship_move(Ship* s, float distance, float direction_rads) {
    if(!s) return -1.0f;
    if (s->consumption_per_km <= 0.0f) return -1.0f;
    // if not anaugh fuel , stop where it should (distance * s->consumption_per_km)
    if(distance * s->consumption_per_km >= s->fuel) {
        distance = ship_get_max_range(s);
    }
    // Calc new position
    s->position.x += distance * cos(direction_rads);
    s->position.y += distance * sin(direction_rads);
    // calc new fuel
    s->fuel -= distance * s->consumption_per_km;
    if (s->fuel < 0.0f) s->fuel = 0.0f;
    return distance;
}


int ship_refuel(Ship* s, float amount) {
    if(!s) return -1;
    if(s->fuel + amount >= s->max_fuel) s->fuel = s->max_fuel;
    else s->fuel += amount;
    return 0;
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

    vec_push(ship->cargo, item);

    return 0;
}

int ship_cargo_remove(Ship* ship, void* obj) {
    if (!ship || !ship->cargo) return -1;

    vec_t* v = ship->cargo;

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
    vec_t* soldiers = vec_create(10);
    vec_t* ships = vec_create(10);
    Soldier* s1 = soldier_create(soldiers, "Ali BOUZIDI", RANK_ADMIRAL, 78.5f);
    Soldier* s2 = soldier_create(soldiers, "Ilyes BOUZIDI", RANK_CAPTAIN, 14.5f);
    Soldier* s3 = soldier_create(soldiers, "Ines TISS", RANK_SEAMN, 68.f);

    Ship* sh1 = ship_create(ships, "OMAR IBN KHATTAB", 1024, 10000000.f, 10.f, 100.f, 100000.f);

    Weapon* w1 = weapon_create("Kruz", 8, 100, 1000.f);

    ship_cargo_add(sh1, CARGO_SOLDIER, s1, s1->weight);
    ship_cargo_add(sh1, CARGO_SOLDIER, s2, s2->weight);
    ship_cargo_add(sh1, CARGO_SOLDIER, s3, s3->weight);
    ship_cargo_add(sh1, CARGO_WEAPON, w1, w1->weight);
    

    vec_print(sh1->cargo, print_cargo);
    // ____________________ Clean up ________________________________
    free(s1); s1 = NULL;
    free(s2); s2 = NULL;
    free(s3); s3 = NULL;

    free(sh1); sh1 = NULL;

    vec_destroy(soldiers);
    vec_destroy(ships);
}


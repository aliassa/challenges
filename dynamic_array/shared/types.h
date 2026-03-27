#ifndef SG_TYPES_H
#define SG_TYPES_H

#define MAX_NAME_LENGTH 32

typedef enum {
    ATTACK_DESTROYED,
    ATTACK_DAMAGED,
    ATTACK_OUT_OF_RANGE,
    ATTACK_OUT_OF_MUNITION,
    ATTACK_INVALID_INPUT
} AttackRes;

typedef enum {
    CARGO_SOLDIER,
    CARGO_STAFF,
    CARGO_WEAPON,
    CARGO_FOOD,
    CARGO_COUNT,
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

typedef struct {
    float x;
    float y;
} Position;


#endif // SG_TYPES_H
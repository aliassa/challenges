// game_loop.c
#include "game_loop.h"
#include "command.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>

void game_loop_run(GameState* gs) {
    char input[128];

    while (!game_is_over(gs)) {
        
        print_status(gs);
        printf("[Turn %u] > ", gs->turn);

        if (!fgets(input, sizeof input, stdin)) break;

        input[strcspn(input, "\n")] = '\0'; // Remove trailing new line

        Command cmd = parse_command(input);
        execute_command(gs, &cmd);

        if (cmd.type == CMD_QUIT) break;

        gs->turn++;
    }
}

int game_is_over(const GameState* gs) {
    // game ends when the player has no ships left
    return gs->ships->length == 0;
}
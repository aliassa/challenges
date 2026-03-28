// game_loop.h
#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "game_state.h"

void game_loop_run(GameState* gs);
int  game_is_over(const GameState* gs);

#endif
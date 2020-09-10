#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "./types.h"

#define INSERT_POW_MAX 2

#define WIN_SCORE 2048

#define START_CELLS_MIN 2
#define START_CELLS_MAX 2

void game_board_clear(game_store* store);
bool game_board_full(game_store* store);
void game_action_add(game_store* store, const board_action_type action, const val value, const dim rt, const dim ct, const dim r1, const dim c1, const dim r2, const dim c2);
bool game_action_undo_last(game_store* store);
void board_action_insert(game_store* store, const val value, const dim rt, const dim ct);
void board_action_move(game_store* store, const val value, const dim rt, const dim ct, const dim r1, const dim c1);
void board_action_add(game_store* store, const val value, const dim rt, const dim ct, const dim r1, const dim c1, const dim r2, const dim c2);
bool game_board_insert_random(game_store* store);
void game_board_move_direction(const game_move move, const dim rows, const dim cols, board_move* m);
bool game_board_move_next(const game_move move, const board_move* m, dim* i, dim* j);
bool game_board_move(game_store* store, const game_move move);
void game_initialize(game_store* store, const dim rows, const dim cols);
bool game_make_move(game_store* store, const game_move move);
void game_print(game_store* store);
#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define DIM_MIN 2
#define DIM_MAX 6

#define INSERT_BASE 2
#define INSERT_POW_MAX 2

#define WIN_CELL_MAX 2048

#define START_CELLS_MIN 2
#define START_CELLS_MAX 3

typedef enum {
    QUIT = 0,
    PLAYING = 1,
    WON = 2,
    LOST = 3,
    WON_END = 4
} game_state;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} game_move;

typedef uint32_t val;
typedef int8_t dim;

typedef val game_board[DIM_MAX][DIM_MAX];

typedef struct {
    dim row_start, row_end,
        col_start, col_end,
        row_inc, col_inc;
} board_move;

typedef enum {
    INSERT = 0,
    MOVE = 1,
    ADD = 2
} board_action_type;

typedef struct {
    board_action_type action;
    val value;
    dim row_target, col_target,
        row_el1, col_el1,
        row_el2, col_el2;
} game_action;

typedef struct {
    val score;
    val base;
    val victory;
    dim rows, cols;
    game_state state;
    game_board board;
    dim actions_count;
    game_action actions[DIM_MAX*DIM_MAX*3];
} game_store;

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
bool game_board_move(game_store* store, const game_move move, val* cell_max);
void game_initialize(game_store* store, const dim rows, const dim cols);
bool game_make_move(game_store* store, const game_move move);
void game_print(game_store* store);
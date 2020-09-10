#pragma once

#include <stdint.h>
#include <stdbool.h>

#define DIM_MIN 2
#define DIM_MAX 16

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
    dim rows, cols;
    game_state state;
    game_board board;
    dim actions_count;
    game_action actions[DIM_MAX*DIM_MAX*3];
} game_store;
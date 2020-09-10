#pragma once

#include <stdint.h>
#include <stdbool.h>

#define DIM_MIN 2
#define DIM_MAX 16

#define INSERT_POW_MAX 2

#define WIN_SCORE 2048

typedef enum {
    PLAYING = 0,
    LOST = 1,
    WON = 2,
    QUIT = 3
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

typedef struct {
    val score;
    dim rows, cols;
    game_state state;
    game_board board;
} game_store;
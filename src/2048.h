#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_DIM 4
#define RND_POW_MAX 2
#define INIT_CELL_MAX 5

typedef enum {
  LOST,
  PLAYING,
  WON,
  OUT_OF_MOVES
} game_state;

typedef enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} direction;

typedef __uint16_t val;
typedef __uint8_t dim;
typedef __int8_t sdim;

typedef val game_board[BOARD_DIM][BOARD_DIM];

typedef struct {
  dim row, col;
} board_cell;

typedef struct {
  val value;
  dim f_row, f_col, t_row, t_col;
} cell_shift;

typedef struct {
  cell_shift cells [2 * BOARD_DIM * BOARD_DIM];
  dim length;
} board_shift;

typedef struct {
  game_state state;
  game_board board;
  val score;
  board_shift shift;
} game_store;
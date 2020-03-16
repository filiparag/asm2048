#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARD_DIM 4
#define RND_POW_MAX 2
#define INIT_CELL_MAX 5

typedef enum {
  LOST, PLAYING
} game_state;

typedef enum {
  UP, DOWN, LEFT, RIGHT
} direction;

typedef __uint32_t val;
typedef __uint8_t dim;

typedef val game_board[BOARD_DIM][BOARD_DIM];

typedef struct {
  dim row, col;
} board_cell;

typedef struct {
  game_state state;
  game_board board;
  val score;
} game_store;
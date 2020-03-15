#include <stdio.h>
#include <stdbool.h>

#define BOARD_DIM 4

typedef enum {
  INITIAL, PLAYING, LOST
} game_state;

typedef enum {
  UP, DOWN, LEFT, RIGHT
} direction;

typedef enum {
  ROW, COLUMN
} axis;

typedef __uint16_t val;
typedef __uint8_t dim;

typedef val game_board[BOARD_DIM][BOARD_DIM];

typedef struct {
  game_state state;
  game_board board;
  val score;
} game_store;

void board_print(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = 0; c < BOARD_DIM; ++c) {
      printf("%4i", board[r][c]);
    }
    printf("\n");
  }
}

void board_set(game_board board, val value, dim r, dim c) {
  board[r][c] = value;
}

dim index_first(direction dir, dim border) {
  if (dir == UP || dir == LEFT)
    return 0;
  else
    return BOARD_DIM - border;
}

dim index_last(direction dir, dim border) {
  if (dir == UP || dir == LEFT)
    return BOARD_DIM - border;
  else
    return 0;
}

__int8_t index_next(direction dir) {
  if (dir == UP || dir == LEFT)
    return 1;
  else
    return -1;
}

val rows_add(game_board board, direction dir) {
  val score = 0;
  dim first = index_first(dir, 1);
  dim last = index_last(dir, 1);
  __int8_t next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c) {
    for (dim r = first; r != last; r += next) {
      if (board[r][c] == board[r+next][c]) {
        board[r][c] *= 2;
        board[r+next][c] = 0;
        score += board[r][c];
      }
    }
  }
  return score;
}

val columns_add(game_board board, direction dir) {
  val score = 0;
  dim first = index_first(dir, 1);
  dim last = index_last(dir, 1);
  __int8_t next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = first; c != last; c += next) {
      if (board[r][c] == board[r][c+next]) {
        board[r][c] *= 2;
        board[r][c+next] = 0;
        score += board[r][c];
      }
    }
  }
  return score;
}

void rows_move(game_board board, direction dir) {
  dim border = (dir == UP) ? 0 : 1;
  dim first = index_first(dir, border);
  dim last = index_last(dir, 0);
  __int8_t next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c) {
    for (dim r = first; r != last; r += next) {
      __int8_t r_next = r + next;
      while (board[r][c] == 0 && r_next + border != last) {
        board[r][c] = board[r_next][c];
        board[r_next][c] = 0;
        r_next += next;
      }
    }
  }
}

void columns_move(game_board board, direction dir) {
  dim border = (dir == LEFT) ? 0 : 1;
  dim first = index_first(dir, border);
  dim last = index_last(dir, 0);
  __int8_t next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = first; c != last; c += next) {
      __int8_t c_next = c + next;
      while (board[r][c] == 0 && c_next + border != last) {
        board[r][c] = board[r][c_next];
        board[r][c_next] = 0;
        c_next += next;
      }
    }
  }
}

void game_action(game_store *game, direction dir) {
  if (dir == LEFT || dir == RIGHT) {
    columns_move(game->board, dir);
    game->score += columns_add(game->board, dir);
    columns_move(game->board, dir);
  } else {
    rows_move(game->board, dir);
    game->score += rows_add(game->board, dir);
    rows_move(game->board, dir);
  }
}

int main2048() {
  game_store game = {INITIAL};
  board_set(game.board, 16, 0, 0);
  board_set(game.board, 16, 0, 2);
  board_set(game.board, 4, 0, 3);
  board_set(game.board, 4, 1, 2);
  board_set(game.board, 2, 1, 0);
  board_set(game.board, 2, 2, 0);
  board_set(game.board, 2, 2, 1);
  board_set(game.board, 2, 2, 3);
  board_set(game.board, 4, 3, 0);
  board_set(game.board, 4, 3, 1);
  board_set(game.board, 4, 3, 2);
  board_set(game.board, 4, 3, 3);
  // board_set(game.board, 2, 0, 0);
  // board_set(game.board, 4, 0, 3);
  board_print(game.board);
  printf("\n");
  game_action(&game, DOWN);
  board_print(game.board);
  printf("\n");
  printf("Score: %i\n", game.score);
  return 0;
}

#include "2048.h"

void board_print(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = 0; c < BOARD_DIM; ++c) {
      printf("%4i", board[r][c]);
    }
    printf("\n");
  }
}

void board_clear(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      board[r][c] = 0;
}

dim board_empty(game_board board, board_cell empty[]) {
  dim count = 0;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      if (board[r][c] == 0) {
        empty[count] = (board_cell) {
          .row = r, .col = c
        };
        ++count;
      }
  return count;
}

val board_max(game_board board) {
  val max = 0;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      if (board[r][c] > max)
        max = board[r][c];
  return max;
}

bool board_insert(game_board board, board_delta* delta) {
  board_cell empty[BOARD_DIM * BOARD_DIM];
  const dim count = board_empty(board, empty);
  if (count > 0) {
    const dim cell = random_cell(count);
    const val value = random_value();
    board[empty[cell].row][empty[cell].col] = value;
    delta_insert(
      delta, value,
      empty[cell].row, empty[cell].col
    );
    return true;
  } else
    return false;
}

val board_add(game_board board, const direction dir,
              board_delta* delta) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const sdim next = index_next(dir);
  for (dim a = 0; a < BOARD_DIM; ++a)
    for (dim b = first; b != last; b += next) {
      dim r, c, r_next, c_next;
      index_cell(dir, a, b, &r, &c, &r_next, &c_next);
      if (
        board[r][c] != 0 &&
        board[r][c] == board[r_next][c_next]
      ) {
        delta_move(
          delta, board,
          r_next, c_next, r, c, true
        );
        board[r][c] <<= 1;
        board[r_next][c_next] = 0;
        score += board[r][c];
        delta_add(
          delta, board[r][c], r, c
        );
      }
    }
  return score;
}

bool board_move(game_board board, const direction dir,
                board_delta* delta, const bool merge) {
  bool moved = false;
  const dim border = 
    (dir == DIRECTION_UP || dir == DIRECTION_LEFT) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const sdim next = index_next(dir);
  for (dim a = 0; a < BOARD_DIM; ++a)
    for (dim b = first; b != last; b += next) {
      dim r, c, r_next, c_next;
      index_cell(dir, a, b, &r, &c, &r_next, &c_next);
      while (board_move_continue(
        board[r][c], dir, r_next, c_next
      )) {
        if (board[r_next][c_next] != 0) {
          delta_move(
            delta, board,
            r_next, c_next, r, c, merge
          );
          board[r][c] = board[r_next][c_next];
          board[r_next][c_next] = 0;
          moved = true;
          break;
        }
        if (!board_move_next(dir, &r_next, &c_next))
          break;
      }
    }
  return moved;
}

bool board_out_of_moves(game_board board) {
  val score = 0;
  dim count = 0;
  game_board hyp_move_board;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c) {
      hyp_move_board[r][c] = board[r][c];
      if (board[r][c] != 0)
        ++count;
    }
  if (count < BOARD_DIM * BOARD_DIM)
    return false;
  score += board_add(hyp_move_board, DIRECTION_UP, NULL);
  score += board_add(hyp_move_board, DIRECTION_DOWN, NULL);
  score += board_add(hyp_move_board, DIRECTION_LEFT, NULL);
  score += board_add(hyp_move_board, DIRECTION_RIGHT, NULL);
  return score == 0;
}
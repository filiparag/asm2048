#include "2048.h"

void board_print(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = 0; c < BOARD_DIM; ++c) {
      printf("%4i", board[r][c]);
    }
    printf("\n");
  }
}

dim index_first(const direction dir, const dim border) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return 0;
  else
    return BOARD_DIM - border;
}

dim index_last(const direction dir, const dim border) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return BOARD_DIM - border;
  else
    return 0;
}

sdim index_next(const direction dir) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return 1;
  else
    return -1;
}

void delta_move(board_change* change, const val value,
                 const dim ro, const dim co,
                 const dim rd, const dim cd,
                 const bool merge) {
  bool merged = false;
  if (merge)
    for (dim m = 0; m < change->move_len; ++m)
      if (
        change->move[m].dest.value == value &&
        change->move[m].dest.row == ro &&
        change->move[m].dest.col == co
      ) {
        change->move[m].dest = (board_cell) {
          rd, cd, value
        };
        merged = true;
        break;
      }
  if (!merge || !merged) {
    change->move[change->move_len] = (board_cell_pair) {
      {ro, co, value}, {rd, cd, value}
    };
    ++change->move_len;
  }
}

void delta_add(board_change* change, const val value,
                 const dim row, const dim col) {
  change->add[change->add_len] = (board_cell) {
    row, col, value
  };
  ++change->add_len;
}

void delta_insert(board_change* change, const val value,
                 const dim row, const dim col) {
  change->insert[change->insert_len] = (board_cell) {
    row, col, value
  };
  ++change->insert_len;
}

void delta_clear(board_change* change) {
  change->move_len = 0;
  change->add_len = 0;
  change->insert_len = 0;
}

void index_cell(const direction dir,
                const dim a, const dim b,
                dim* r, dim* c, dim* r_next, dim* c_next) {
  switch (dir) {
    case DIRECTION_UP:
      *r = b;
      *c = a;
      *r_next = b + 1;
      *c_next = a;
      break;
    case DIRECTION_DOWN:
      *r = b;
      *c = a;
      *r_next = b - 1;
      *c_next = a;
      break;
    case DIRECTION_LEFT:
      *r = a;
      *c = b;
      *r_next = a;
      *c_next = b + 1;
      break;
    case DIRECTION_RIGHT:
      *r = a;
      *c = b;
      *r_next = a;
      *c_next = b - 1;
      break;
  }
}

bool board_move_continue(const val cell,
                         const direction dir,
                         const dim r_next, const dim c_next) {
  if (cell != 0)
    return false;
  switch (dir) {
    case DIRECTION_UP:
      return r_next + 0 != BOARD_DIM - 0;
    case DIRECTION_DOWN:
      return r_next + 1 != 0;
    case DIRECTION_LEFT:
      return c_next + 0 != BOARD_DIM - 0;
    case DIRECTION_RIGHT:
      return c_next + 1 != 0;
    default:
      return false;
  }
}

bool board_move_next(const direction dir,
                     dim* r_next, dim* c_next) {
  switch (dir) {
    case DIRECTION_UP:
      *r_next += 1;
      break;
    case DIRECTION_DOWN:
      *r_next -= 1;
      break;
    case DIRECTION_LEFT:
      *c_next += 1;
      break;
    case DIRECTION_RIGHT:
      *c_next -= 1;
      break;
  }
  return *r_next < BOARD_DIM && 
         *c_next < BOARD_DIM;
}

val board_add(game_board board, const direction dir) {
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
        board[r][c] <<= 1;
        board[r_next][c_next] = 0;
        score += board[r][c];
      }
    }
  return score;
}

bool board_move(game_board board, const direction dir) {
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
          board[r][c] = board[r_next][c_next];
          board[r_next][c_next] = 0;
          moved = true;
        }
        if (!board_move_next(dir, &r_next, &c_next))
          break;
      }
    }
  return moved;
}

val random_value() {
  return 1 << (rand() % RND_POW_MAX + 1);
}

dim random_cell(const dim count) {
  return rand() % count;
}

bool board_insert(game_board board, board_change* change) {
  board_cell empty[BOARD_DIM * BOARD_DIM];
  dim count = 0;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      if (board[r][c] == 0) {
        empty[count] = (board_cell){.row = r, .col = c};
        ++count;
      }
  if (count > 0) {
    const dim cell = random_cell(count);
    const val value = random_value();
    board[empty[cell].row][empty[cell].col] = value;
    delta_insert(
      change, value,
      empty[cell].row, empty[cell].col
    );
    return true;
  } else
    return false;
}

void board_clear(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      board[r][c] = 0;
}

val board_max(game_board board) {
  val max = 0;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      if (board[r][c] > max)
        max = board[r][c];
  return max;
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
  score += board_add(hyp_move_board, DIRECTION_LEFT);
  score += board_add(hyp_move_board, DIRECTION_RIGHT);
  score += board_add(hyp_move_board, DIRECTION_UP);
  score += board_add(hyp_move_board, DIRECTION_DOWN);
  return score == 0;
}

void game_initialize(game_store *game) {
  board_clear(game->board);
  delta_clear(&game->delta);
  const dim count = random_cell(INIT_CELL_MAX) + 1;
  for (dim c = 0; c < count; ++c)
    board_insert(game->board, &game->delta);
  game->state = STATE_PLAYING;
  game->score = 0;
}

void game_action(game_store *game, const direction dir) {
  if (game->state == STATE_LOST || game->state == STATE_OUT_OF_MOVES)
    return;
  bool moved = false;
  delta_clear(&game->delta);
  moved |= board_move(game->board, dir);
  const val scored = board_add(game->board, dir);
  moved |= board_move(game->board, dir);
  const val cell_max = board_max(game->board);
  game->score += scored;
  if (moved || scored > 0) {
    if (cell_max >= 2048)
      game->state = STATE_WON;
    board_insert(game->board, &game->delta);
  } else if (!moved && board_out_of_moves(game->board)) {
    if (cell_max < 2048)
      game->state = STATE_LOST;
    else
      game->state = STATE_OUT_OF_MOVES;
  }
}

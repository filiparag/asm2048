#ifndef HEADER_LOGIC
  #define HEADER_LOGIC
  #include "2048.h"
#endif

void board_print(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r) {
    for (dim c = 0; c < BOARD_DIM; ++c) {
      printf("%4i", board[r][c]);
    }
    printf("\n");
  }
}

dim inline index_first(const direction dir, const dim border) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return 0;
  else
    return BOARD_DIM - border;
}

dim inline index_last(const direction dir, const dim border) {
  if (dir == DIRECTION_UP || dir == DIRECTION_LEFT)
    return BOARD_DIM - border;
  else
    return 0;
}

sdim inline index_next(const direction dir) {
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

val rows_add(game_board board, const direction dir,
             board_change* change) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const sdim next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c)
    for (dim r = first; r != last; r += next)
      if (
        board[r][c] != 0 &&
        board[r][c] == board[r+next][c]
      ) {
        if (change != NULL) {
          delta_move(change, board[r][c], r+next, c, r, c, true);
          delta_add(change, board[r][c] * 2, r, c);
        }
        board[r][c] *= 2;
        board[r+next][c] = 0;
        score += board[r][c];
      }
  return score;
}

val columns_add(game_board board, const direction dir,
                board_change* change) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const sdim next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = first; c != last; c += next)
      if (
        board[r][c] != 0 &&
        board[r][c] == board[r][c+next]
      ) {
        if (change != NULL) {
          delta_move(change, board[r][c], r, c+next, r, c, true);
          delta_add(change, board[r][c] * 2, r, c);
        }
        board[r][c] *= 2;
        board[r][c+next] = 0;
        score += board[r][c];
      }
  return score;
}

bool rows_move(game_board board, const direction dir,
               board_change* change) {
  bool moved = false;
  dim border = (dir == DIRECTION_UP) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const sdim next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c)
    for (dim r = first; r != last; r += next) {
      sdim r_next = r + next;
      while (board[r][c] == 0 && r_next + border != last) {
        if (board[r_next][c] != 0) {
          board[r][c] = board[r_next][c];
          board[r_next][c] = 0;
          moved = true;
          if (change != NULL)
            delta_move(change, board[r][c], r_next, c, r, c, false);
        }
        r_next += next;
      }
    }
  return moved;
}

bool columns_move(game_board board, const direction dir,
                  board_change* change) {
  bool moved = false;
  dim border = (dir == DIRECTION_LEFT) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const sdim next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = first; c != last; c += next) {
      sdim c_next = c + next;
      while (board[r][c] == 0 && c_next + border != last) {
        if (board[r][c_next] != 0) {
          board[r][c] = board[r][c_next];
          board[r][c_next] = 0;
          moved = true;
          if (change != NULL)
            delta_move(change, board[r][c], r, c_next, r, c, false);
        }
        c_next += next;
      }
    }
  return moved;
}

val inline random_value() {
  return 1 << (rand() % RND_POW_MAX + 1);
}

dim inline random_cell(const dim count) {
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
  score += columns_add(hyp_move_board, DIRECTION_LEFT, NULL);
  score += columns_add(hyp_move_board, DIRECTION_RIGHT, NULL);
  score += rows_add(hyp_move_board, DIRECTION_UP, NULL);
  score += rows_add(hyp_move_board, DIRECTION_DOWN, NULL);
  return score == 0;
}

void game_initialize(game_store *game) {
  board_clear(game->board);
  delta_clear(&game->delta);
  const dim count = random_cell(INIT_CELL_MAX) + 1;
  for (dim c = 0; c < count; ++c)
    board_insert(game->board, &game->delta);
  game->state = PLAYING;
  game->score = 0;
}

void game_action(game_store *game, const direction dir) {
  if (game->state == LOST || game->state == OUT_OF_MOVES)
    return;
  bool moved = false;
  delta_clear(&game->delta);
  if (dir == DIRECTION_LEFT || dir == DIRECTION_RIGHT) {
    moved |= columns_move(game->board, dir, &game->delta);
    game->score += columns_add(game->board, dir, &game->delta);
    moved |= columns_move(game->board, dir, &game->delta);
  } else {
    moved |= rows_move(game->board, dir, &game->delta);
    game->score += rows_add(game->board, dir, &game->delta);
    moved |= rows_move(game->board, dir, &game->delta);
  }
  const val cell_max = board_max(game->board);
  if (moved) {
    if (cell_max >= 2048)
      game->state = WON;
    board_insert(game->board, &game->delta);
  } else if (!moved && board_out_of_moves(game->board)) {
    if (cell_max < 2048)
      game->state = LOST;
    else
      game->state = OUT_OF_MOVES;
  }
}

void game_play_console() {
  srand(time(NULL));
  game_store game;
  game_initialize(&game);
  while (true) {
    
    printf("Score: %9i\n\n", game.score);
    board_print(game.board);
    if (game.state == LOST)
      printf("\nYou lost!\n");

    printf("\n:");
    char input;
    scanf("%c", &input);
    switch (input) {
      case 'q':
        return;
      case 'u':
        game_action(&game, DIRECTION_UP);
        break;
      case 'd':
        game_action(&game, DIRECTION_DOWN);
        break;
      case 'l':
        game_action(&game, DIRECTION_LEFT);
        break;
      case 'r':
        game_action(&game, DIRECTION_RIGHT);
        break;
      case 'n':
        game_initialize(&game);
      case 'f':
        delta_clear(&game.delta);
        board_insert(game.board, &game.delta);
        break;
      default:
        break;
    }

    printf("\n");
    board_print(game.board);
    printf("\n");

    for (dim m = 0; m < game.delta.move_len; ++m)
      printf(
        "\n%5i (%i, %i) -> (%i, %i)",
        game.delta.move[m].orig.value,
        game.delta.move[m].orig.row,
        game.delta.move[m].orig.col,
        game.delta.move[m].dest.row,
        game.delta.move[m].dest.col
      );

    scanf("\n");
    printf("\x1B[1;1H\x1B[2J");
    
  }
}

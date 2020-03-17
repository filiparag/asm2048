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

void board_shift_cell(board_shift* shift, const val value,
                      const dim r_from, const dim c_from,
                      const dim r_to, const dim c_to) {
  shift->cells[shift->length] = (cell_shift) {
    value, r_from, c_from, r_to, c_to
  };
  ++shift->length;
}

void inline board_shift_clear(board_shift* shift) {
  shift->length = 0;
}

val rows_add(game_board board, const direction dir,
             board_shift* shift) {
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
        // if (shift != NULL)
        //   board_shift_cell(shift, board[r][c], r+next, c, r, c);
        board[r][c] *= 2;
        board[r+next][c] = 0;
        score += board[r][c];
      }
  return score;
}

val columns_add(game_board board, const direction dir,
                board_shift* shift) {
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
        // if (shift != NULL)
        //   board_shift_cell(shift, board[r][c], r+next, c, r, c);
        board[r][c] *= 2;
        board[r][c+next] = 0;
        score += board[r][c];
      }
  return score;
}

bool rows_move(game_board board, const direction dir,
               board_shift* shift) {
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
          if (shift != NULL)
            board_shift_cell(shift, board[r][c], r_next, c, r, c);
        }
        r_next += next;
      }
    }
  return moved;
}

bool columns_move(game_board board, const direction dir,
                  board_shift* shift) {
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
          if (shift != NULL)
            board_shift_cell(shift, board[r][c], r, c_next, r, c);
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

bool board_insert(game_board board) {
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
    board[empty[cell].row][empty[cell].col] = random_value();
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
  game_board hyp_move_board;
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      hyp_move_board[r][c] = board[r][c];
  score += columns_add(hyp_move_board, DIRECTION_LEFT, NULL);
  score += columns_add(hyp_move_board, DIRECTION_RIGHT, NULL);
  score += rows_add(hyp_move_board, DIRECTION_UP, NULL);
  score += rows_add(hyp_move_board, DIRECTION_DOWN, NULL);
  return score == 0;
}

void game_initialize(game_store *game) {
  board_clear(game->board);
  board_shift_clear(&game->shift);
  const dim count = random_cell(INIT_CELL_MAX) + 1;
  for (dim c = 0; c < count; ++c)
    board_insert(game->board);
  game->state = PLAYING;
}

void game_action(game_store *game, const direction dir) {
  if (game->state == LOST || game->state == OUT_OF_MOVES)
    return;
  bool moved = false;
  board_shift_clear(&game->shift);
  if (dir == DIRECTION_LEFT || dir == DIRECTION_RIGHT) {
    moved |= columns_move(game->board, dir, &game->shift);
    game->score += columns_add(game->board, dir, &game->shift);
    moved |= columns_move(game->board, dir, &game->shift);
  } else {
    moved |= rows_move(game->board, dir, &game->shift);
    game->score += rows_add(game->board, dir, &game->shift);
    moved |= rows_move(game->board, dir, &game->shift);
  }
  const val cell_max = board_max(game->board);
  if (!board_insert(game->board)) {
    if (!moved && board_out_of_moves(game->board)) {
      if (cell_max < 2048)
        game->state = LOST;
      else
        game->state = OUT_OF_MOVES;
    }
  } else if (cell_max >= 2048)
    game->state = WON;
}

void game_play_console() {
  srand(time(NULL));
  game_store game;
  game_initialize(&game);
  while (true) {
    printf("\x1B[1;1H\x1B[2J");
    printf("Score: %9i\n\n", game.score);
    board_print(game.board);

    for (dim i = 0; i < game.shift.length; ++i)
      printf(
        "%4i [ %i, %i ] -> [ %i, %i ]\n",
        game.shift.cells[i].value,
        game.shift.cells[i].f_row,
        game.shift.cells[i].f_col,
        game.shift.cells[i].t_row,
        game.shift.cells[i].t_col
      );
    
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
        break;
      default:
        break;
    }
  }
}

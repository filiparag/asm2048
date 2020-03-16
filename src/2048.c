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

dim inline index_first(direction dir, dim border) {
  if (dir == UP || dir == LEFT)
    return 0;
  else
    return BOARD_DIM - border;
}

dim inline index_last(direction dir, dim border) {
  if (dir == UP || dir == LEFT)
    return BOARD_DIM - border;
  else
    return 0;
}

__int8_t inline index_next(direction dir) {
  if (dir == UP || dir == LEFT)
    return 1;
  else
    return -1;
}

val rows_add(game_board board, direction dir) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const __int8_t next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c)
    for (dim r = first; r != last; r += next)
      if (board[r][c] == board[r+next][c]) {
        board[r][c] *= 2;
        board[r+next][c] = 0;
        score += board[r][c];
      }
  return score;
}

val columns_add(game_board board, direction dir) {
  val score = 0;
  const dim first = index_first(dir, 1);
  const dim last = index_last(dir, 1);
  const __int8_t next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = first; c != last; c += next)
      if (board[r][c] == board[r][c+next]) {
        board[r][c] *= 2;
        board[r][c+next] = 0;
        score += board[r][c];
      }
  return score;
}

bool rows_move(game_board board, direction dir) {
  bool moved = false;
  dim border = (dir == UP) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const __int8_t next = index_next(dir);
  for (dim c = 0; c < BOARD_DIM; ++c)
    for (dim r = first; r != last; r += next) {
      __int8_t r_next = r + next;
      while (board[r][c] == 0 && r_next + border != last) {
        if (board[r_next][c] != 0) {
          board[r][c] = board[r_next][c];
          board[r_next][c] = 0;
          moved = true;
        }
        r_next += next;
      }
    }
  return moved;
}

bool columns_move(game_board board, direction dir) {
  bool moved = false;
  dim border = (dir == LEFT) ? 0 : 1;
  const dim first = index_first(dir, border);
  const dim last = index_last(dir, 0);
  const __int8_t next = index_next(dir);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = first; c != last; c += next) {
      __int8_t c_next = c + next;
      while (board[r][c] == 0 && c_next + border != last) {
        if (board[r][c_next] != 0) {
          board[r][c] = board[r][c_next];
          board[r][c_next] = 0;
          moved = true;
        }
        c_next += next;
      }
    }
  return moved;
}

val inline random_value() {
  return 1 << (rand() % RND_POW_MAX + 1);
}

dim inline random_cell(dim count) {
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

void game_initialize(game_store *game) {
  if (game->state == PLAYING)
    return;
  board_clear(game->board);
  const dim count = random_cell(INIT_CELL_MAX) + 1;
  for (dim c = 0; c < count; ++c)
    board_insert(game->board);
  game->score = 0;
  game->state = PLAYING;
}

void game_action(game_store *game, direction dir) {
  if (game->state != PLAYING)
    return;
  bool moved = false;
  if (dir == LEFT || dir == RIGHT) {
    moved |= columns_move(game->board, dir);
    game->score += columns_add(game->board, dir);
    moved |= columns_move(game->board, dir);
  } else {
    moved |= rows_move(game->board, dir);
    game->score += rows_add(game->board, dir);
    moved |= rows_move(game->board, dir);
  }
  if (moved && !board_insert(game->board))
    game->state = LOST;
}

int main2048() {
  srand(time(NULL));
  game_store game = {LOST};
  game_initialize(&game);
  while (true) {
    printf("\x1B[1;1H\x1B[2J");
    printf("Score: %9i\n\n", game.score);
    board_print(game.board);
    if (game.state == LOST)
      printf("\nYou lost!\n");
    printf("\n:");
    char input;
    scanf("%c", &input);
    switch (input) {
      case 'q':
        return 0;
      case 'u': {
        game_action(&game, UP);
        break;
      }
      case 'd': {
        game_action(&game, DOWN);
        break;
      }
      case 'l': {
        game_action(&game, LEFT);
        break;
      }
      case 'r': {
        game_action(&game, RIGHT);
        break;
      }
      case 'n': {
        game_initialize(&game);
        break;
      }
      default:
        break;
    }
  }
  return 0;
}

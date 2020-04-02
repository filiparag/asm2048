#include "2048.h"

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
  if (
    game->state == STATE_LOST ||
    game->state == STATE_OUT_OF_MOVES
  )
    return;

  printf("\x1B[1;1H\x1B[2J");

  bool moved = false;
  delta_clear(&game->delta);
  moved |= board_move(game->board, dir, &game->delta, false);
  const val scored = board_add(game->board, dir, &game->delta);
  moved |= board_move(game->board, dir, &game->delta, true);
  const val cell_max = board_max(game->board);
  game->score += scored;

  if (game->delta.move_len > 0)
    printf("Move:\n");
  for (dim i = 0; i < game->delta.move_len; ++i)
    printf(
      "%4i (%i, %i) -> %4i (%i, %i)\n",
      game->delta.move[i].orig.value,
      game->delta.move[i].orig.row,
      game->delta.move[i].orig.col,
      game->delta.move[i].dest.value,
      game->delta.move[i].dest.row,
      game->delta.move[i].dest.col
    );

  if (game->delta.add_len > 0)
    printf("\nAdd:\n");
  for (dim i = 0; i < game->delta.add_len; ++i)
    printf(
      "%4i (%i, %i)\n",
      game->delta.add[i].value,
      game->delta.add[i].row,
      game->delta.add[i].col
    );

  if (moved || scored > 0) {
    if (cell_max >= 2048)
      game->state = STATE_WON;
    board_insert(game->board, &game->delta);

    if (game->delta.insert_len > 0)
      printf("\nInsert:\n");
    for (dim i = 0; i < game->delta.insert_len; ++i)
      printf(
        "%4i (%i, %i)\n",
        game->delta.insert[i].value,
        game->delta.insert[i].row,
        game->delta.insert[i].col
      );

  } else if (!moved && board_out_of_moves(game->board)) {
    if (cell_max < 2048)
      game->state = STATE_LOST;
    else
      game->state = STATE_OUT_OF_MOVES;
  }
}
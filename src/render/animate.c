#include "animate.h"

void animate_clear() {
  move_len = 0;
  add_len = 0;
  insert_len = 0;
}

void animate_flush_completed() {
  scale_cell insert_r[BUFFER_SIZE];
  dim insert_r_len = 0;
  for (dim i = 0; i < insert_len; ++i) {
    if (
      insert[i].elapsed_time <=
      ANIM_INSERT_DELAY + ANIM_INSERT_DURATION
    ) {
      insert_r[insert_r_len] = (scale_cell) {
        .value = insert[i].value,
        .row = insert[i].row,
        .col = insert[i].col,
        .elapsed_time = insert[i].elapsed_time
      };
      ++insert_r_len;
    } else {
      board_render[insert[i].row][insert[i].col] =
        insert[i].value;
    }
  }
  for (dim i = 0; i < insert_r_len; ++i) {
    insert[i] = (scale_cell) {
      .value = insert_r[i].value,
      .row = insert_r[i].row,
      .col = insert_r[i].col,
      .elapsed_time = insert_r[i].elapsed_time
    };
  }
  insert_len = insert_r_len;

  scale_cell add_r[BUFFER_SIZE];
  dim add_r_len = 0;
  for (dim i = 0; i < add_len; ++i) {
    if (
      add[i].elapsed_time <=
      ANIM_ADD_DELAY + ANIM_ADD_DURATION
    ) {
      add_r[add_r_len] = (scale_cell) {
        .value = add[i].value,
        .row = add[i].row,
        .col = add[i].col,
        .elapsed_time = add[i].elapsed_time
      };
      ++add_r_len;
    } else {
      board_render[add[i].row][add[i].col] =
        add[i].value;
    }
  }
  for (dim i = 0; i < add_r_len; ++i) {
    add[i] = (scale_cell) {
      .value = add_r[i].value,
      .row = add_r[i].row,
      .col = add_r[i].col,
      .elapsed_time = add_r[i].elapsed_time
    };
  }
  add_len = add_r_len;

  translation_cell move_r[BUFFER_SIZE];
  dim move_r_len = 0;
  for (dim i = 0; i < move_len; ++i) {
    if (
      move[i].elapsed_time <=
      ANIM_MOVE_DELAY + ANIM_MOVE_DURATION
    ) {
      move_r[move_r_len] = (translation_cell) {
        .value = move[i].value,
        .x = move[i].x,
        .y = move[i].y,
        .orig = {
          .row = move[i].orig.row,
          .col = move[i].orig.col
        },
        .dest = {
          .row = move[i].dest.row,
          .col = move[i].dest.col
        },
        .elapsed_time = move[i].elapsed_time
      };
      ++move_r_len;
    } else {
      board_render[move[i].dest.row][move[i].dest.col] =
        move[i].value;
    }
  }
  for (dim i = 0; i < move_r_len; ++i) {
    move[i] = (translation_cell) {
      .value = move_r[i].value,
      .x = move_r[i].x,
      .y = move_r[i].y,
      .orig = {
        .row = move_r[i].orig.row,
        .col = move_r[i].orig.col
      },
      .dest = {
        .row = move_r[i].dest.row,
        .col = move_r[i].dest.col
      },
      .elapsed_time = move_r[i].elapsed_time
    };
  }
  move_len = move_r_len;
}

bool animate_in_progress() {
  return move_len > 0 || insert_len > 0 || add_len > 0;
}

bool animate_board(SDL_Renderer* ren,
                   const double delta_time,
                   game_board board, game_board board_old,
                   board_delta* delta) {
  
  if (
    move_len == 0 &&
    insert_len == 0 &&
    add_len == 0
  ) {
    if (
      delta->move_len == 0 &&
      delta->insert_len == 0 &&
      delta->add_len == 0
    )
      return false;
    else
      board_snapshot(board_old);
  }

  debug(ren, 1000 / delta_time, 0);

  animate_flush_completed();

  move_append(delta->move, delta->move_len);
  add_append(delta->add, delta->add_len);
  insert_append(delta->insert, delta->insert_len);

  delta_clear(delta);
  
  move_update(delta_time);
  add_update(delta_time);
  insert_update(delta_time);

  draw_cells(ren, board_render, 1);

  move_render(ren);
  add_render(ren);
  insert_render(ren);
  
  return move_len > 0 || insert_len > 0 || add_len > 0;
}
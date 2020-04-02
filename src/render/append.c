#include "animate.h"

void move_append(const board_cell_pair cells[],
                 const dim len) {
  for (dim i = 0; i < len; ++i) {
    pos x, y;
    dim_to_pos(
      cells[i].orig.row,
      cells[i].orig.col,
      &x,
      &y
    );
    move[move_len] = (translation_cell) {
      .value = cells[i].orig.value,
      .x = x,
      .y = y,
      .orig = {
        .row = cells[i].orig.row,
        .col = cells[i].orig.col
      },
      .dest = {
        .row = cells[i].dest.row,
        .col = cells[i].dest.col
      },
      .elapsed_time = 0
    };
    ++move_len;
    dim r_from, r_to, c_from, c_to;
    if (move[i].orig.row < move[i].dest.row) {
      r_from = move[i].orig.row;
      r_to = move[i].dest.row;
    } else {
      r_from = move[i].dest.row;
      r_to = move[i].orig.row;
    }
    if (move[i].orig.col < move[i].dest.col) {
      c_from = move[i].orig.col;
      c_to = move[i].dest.col;
    } else {
      c_from = move[i].dest.col;
      c_to = move[i].orig.col;
    }
    if (
      board_render[move[i].dest.row]
        [move[i].dest.col] == move[i].dest.value
    )
      board_move_keep[move[i].dest.row]
        [move[i].dest.col] = true;
    for (dim r = r_from; r <= r_to; ++r)
      for (dim c = c_from; c <= c_to; ++c)
        board_render[r][c] = 0;
  }
}

void insert_append(const board_cell cells[],
                   const dim len) {
  for (dim i = 0; i < len; ++i) {
    insert[insert_len] = (scale_cell) {
      .value = cells[i].value,
      .row = cells[i].row,
      .col = cells[i].col,
      .scale = ANIM_INSERT_SCALE,
      .elapsed_time = 0
    };
    ++insert_len;
    board_render[cells[i].row][cells[i].col] = 0;
  }
}

void add_append(const board_cell cells[],
                const dim len) {
  for (dim i = 0; i < len; ++i) {
    add[add_len] = (scale_cell) {
      .value = cells[i].value,
      .row = cells[i].row,
      .col = cells[i].col,
      .scale = ANIM_ADD_SCALE,
      .elapsed_time = 0
    };
    ++add_len;
    board_render[cells[i].row][cells[i].col] = 0;
  }
}
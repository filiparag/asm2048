#include "animate.h"

void move_update(const double delta_time) {
  for (dim i = 0; i < move_len; ++i) {
    if (move[i].elapsed_time >= ANIM_MOVE_DELAY) {
      const double ratio = transition(
        move[i].elapsed_time - ANIM_MOVE_DELAY,
        ANIM_MOVE_DURATION,
        ANIM_MOVE_STRENGTH,
        TRANSITION_SIGMOID
      );
      pos orig_x, orig_y, dest_x, dest_y;
      dim_to_pos(
        move[i].orig.row,
        move[i].orig.col,
        &orig_x,
        &orig_y
      );
      dim_to_pos(
        move[i].dest.row,
        move[i].dest.col,
        &dest_x,
        &dest_y
      );
      if (orig_x == dest_x)
        move[i].y = (
          orig_y + (
            (int) dest_y - (int) orig_y
          ) * ratio
        );
      else
        move[i].x = (
          orig_x + (
            (int) dest_x - (int) orig_x
          ) * ratio
        );
    }
    move[i].elapsed_time += delta_time;
  }
}

void insert_update(const double delta_time) {
  for (dim i = 0; i < insert_len; ++i) {
    if (insert[i].elapsed_time >= ANIM_INSERT_DELAY) {
      insert[i].scale = transition(
        insert[i].elapsed_time - ANIM_INSERT_DELAY,
        ANIM_INSERT_DURATION,
        ANIM_INSERT_STRENGTH,
        TRANSITION_SIGMOID
      );
    }
    insert[i].elapsed_time += delta_time;
  }
}

void add_update(const double delta_time) {
  for (dim i = 0; i < add_len; ++i) {
    if (add[i].elapsed_time >= ANIM_ADD_DELAY) {
      add[i].scale = transition(
        add[i].elapsed_time - ANIM_ADD_DELAY,
        ANIM_ADD_DURATION,
        ANIM_ADD_STRENGTH,
        TRANSITION_SQUARE
      );
    } else {
      if (!board_move_keep[add[i].row][add[i].col])
        board_render[add[i].row][add[i].col] =
          add[i].value / 2;
    }
    add[i].elapsed_time += delta_time;
  }
}
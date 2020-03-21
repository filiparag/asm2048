#ifndef HEADER_LOGIC
  #define HEADER_LOGIC
  #include "2048.h"
#endif

#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#ifndef HEADER_ANIMATIONS
  #define HEADER_ANIMATIONS
  #include "animate.h"
#endif

translation_cell move[BUFFER_SIZE];
scale_cell insert[BUFFER_SIZE];
scale_cell add[BUFFER_SIZE];
dim move_len = 0, add_len = 0, insert_len = 0,
    move_first = 0, add_first = 0, insert_first = 0;

game_board board_render;
bool board_move_keep[BOARD_DIM][BOARD_DIM];

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

void debug(SDL_Renderer* ren,
           const double v, const pos p) {
  static char v_string[24];
  sprintf(v_string, "%5.5f", v);
  draw_text(
    ren, v_string, BOARD_SIZE / 2, BOARD_PADDING + 25 * p,
    0, 25, (SDL_Color){0,0,0}, ALIGN_LEFT
  );
}

double transition(const double curr, const double max,
                  const double strenght,
                  const transition_type type) {
  if (type == TRANSITION_SIGMOID)
    return 1 / (
      1 + exp(-(strenght * (curr / max - 0.5)))
    );
  else if (type == TRANSITION_SQUARE)
    return 1 - (
      0.4 * curr * strenght * (curr - max)
    ) / pow(max, 2);
  else
    return 1;
}

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

void move_render(SDL_Renderer* ren) {
  for (dim i = 0; i < move_len; ++i)
    draw_cell_pos(
      ren,
      move[i].value,
      move[i].x,
      move[i].y,
      1
    );
}

void insert_render(SDL_Renderer* ren) {
  for (dim i = 0; i < insert_len; ++i)
    draw_cell(
      ren,
      insert[i].value,
      insert[i].row,
      insert[i].col,
      insert[i].scale
    );
}

void add_render(SDL_Renderer* ren) {
  for (dim i = 0; i < add_len; ++i)
    draw_cell(
      ren,
      add[i].value,
      add[i].row,
      add[i].col,
      add[i].scale
    );
}

void board_unchanged(game_board new, game_board old) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c) {
      if (
        new[r][c] != 0 &&
        new[r][c] == old[r][c]
      )
        board_render[r][c] = new[r][c];
      else
        board_render[r][c] = 0;
      board_move_keep[r][c] = false;
    }
}

void board_copy(game_board board) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c) {
      board_render[r][c] = board[r][c];
      board_move_keep[r][c] = false;
    }
}

bool animate_board(SDL_Renderer* ren,
                   const double delta_time,
                   game_board board, game_board board_old,
                   board_change* change) {

  if (
    move_len == 0 &&
    insert_len == 0 &&
    add_len == 0
  ) {
    if (
      change->move_len == 0 &&
      change->insert_len == 0 &&
      change->add_len == 0
    )
      return false;
    else
      board_copy(board_old);
  }

  debug(ren, 1000 / delta_time, 0);

  animate_flush_completed();

  move_append(change->move, change->move_len);
  add_append(change->add, change->add_len);
  insert_append(change->insert, change->insert_len);

  delta_clear(change);
  
  move_update(delta_time);
  add_update(delta_time);
  insert_update(delta_time);

  draw_cells(ren, board_render, 1);

  move_render(ren);
  add_render(ren);
  insert_render(ren);
  
  return move_len > 0 || insert_len > 0 || add_len > 0;
}
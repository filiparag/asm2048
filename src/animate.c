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


void animate_clear() {
  move_len = 0;
  add_len = 0;
  insert_len = 0;
}

void animate_flush_completed(game_board board) {
  scale_cell insert_r[BUFFER_SIZE];
  dim insert_r_len = 0;
  for (dim i = 0; i < insert_len; ++i) {
    if (insert[i].elapsed_time <= SCALE_TIME) {
      insert_r[insert_r_len] = (scale_cell) {
        .value = insert[i].value,
        .row = insert[i].row,
        .col = insert[i].col,
        .elapsed_time = insert[i].elapsed_time
      };
      ++insert_r_len;
    } else {
      board[insert[i].row][insert[i].col] =
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

  translation_cell move_r[BUFFER_SIZE];
  dim move_r_len = 0;
  for (dim i = 0; i < move_len; ++i) {
    if (move[i].elapsed_time <= SCALE_TIME) {
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
      board[move[i].dest.row][move[i].dest.col] =
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
  if (type == EASE_SIGMOID) {
      double x = strenght * (curr / max - 0.5);
      return 1 / (1 + exp(-x));
  } else
    return 1;
}

void move_append(game_board board,
                 const board_cell_pair cells[],
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
    for (dim r = move[i].orig.row; r <= move[i].dest.row; ++r)
      for (dim c = move[i].orig.col; c <= move[i].dest.col; ++c)
        board[r][c] = 0;
    // board[move[i].dest.row][move[i].dest.col] = move[i].dest.value;
  }
}

void insert_append(game_board board,
                   const board_cell cells[],
                   const dim len) {
  for (dim i = 0; i < len; ++i) {
    insert[insert_len] = (scale_cell) {
      .value = cells[i].value,
      .row = cells[i].row,
      .col = cells[i].col,
      .scale = SCALE_ORIG,
      .elapsed_time = 0
    };
    ++insert_len;
    board[cells[i].row][cells[i].col] = 0;
  }
}

void move_update(const double delta_time) {
  for (dim i = 0; i < move_len; ++i) {
    const double ratio = transition(
      move[i].elapsed_time,
      TRANSLATE_TIME,
      TRANSLATE_EASE_STRENGTH,
      EASE_SIGMOID
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
    move[i].elapsed_time += delta_time;
  }
}

void insert_update(const double delta_time) {
  for (dim i = 0; i < insert_len; ++i) {
    insert[i].scale = transition(
      insert[i].elapsed_time,
      SCALE_TIME,
      SCALE_EASE_STRENGTH,
      EASE_SIGMOID
    );
    insert[i].elapsed_time += delta_time;
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

void board_unchanged(game_board new, game_board old,
                     game_board unchanged) {
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      if (
        new[r][c] != 0 &&
        new[r][c] == old[r][c]
      )
        unchanged[r][c] = new[r][c];
      else
        unchanged[r][c] = 0;
}

bool animate_board(SDL_Renderer* ren,
                   const double delta_time,
                   game_board board, game_board board_old,
                   board_change* change) {

  static char fps[24];
  sprintf(fps, "%5.3f", 1000 / delta_time);
  draw_text(
    ren, fps, BOARD_SIZE / 2, HEADER_SIZE / 2,
    0, 30, (SDL_Color){0,0,0}, ALIGN_LEFT
  );
  
  game_board board_render;
  board_unchanged(board, board_old, board_render);

  animate_flush_completed(board_render);

  move_append(board_render, change->move, change->move_len);
  insert_append(board_render, change->insert, change->insert_len);

  delta_clear(change);
  
  move_update(delta_time);
  insert_update(delta_time);

  draw_cells(ren, board_render, 1);

  move_render(ren);
  insert_render(ren);

  sprintf(fps, "%5i", move_len);
  draw_text(
    ren, fps, BOARD_SIZE / 2, HEADER_SIZE / 2 + 30,
    0, 30, (SDL_Color){0,0,0}, ALIGN_LEFT
  );
  
  return move_len > 0 || insert_len > 0;
}
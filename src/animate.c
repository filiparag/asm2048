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
translation_cell add[BUFFER_SIZE];
scale_cell insert[BUFFER_SIZE];
dim move_len = 0, add_len = 0, insert_len = 0,
    move_first = 0, add_first = 0, insert_first = 0;

void animate_clear() {
  move_len = 0;
  add_len = 0;
  insert_len = 0;
}

void debug(SDL_Renderer* ren, const double v, const pos p) {
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

void move_append(const board_cell_pair cells[],
                 const dim len) {
  for (dim i = 0; i < len; ++i) {
    const dim index = (move_first + move_len)
                      % BUFFER_SIZE;
    pos orig_x, orig_y, dest_x, dest_y;
    dim_to_pos(
      cells[i].orig.row,
      cells[i].orig.col,
      &orig_x,
      &orig_y
    );
    dim_to_pos(
      cells[i].dest.row,
      cells[i].dest.col,
      &dest_x,
      &dest_y
    );
    move[index] = (translation_cell) {
      .value = cells[i].orig.value,
      .x = orig_x,
      .y = orig_y,
      .orig_x = orig_x,
      .orig_y = orig_y,
      .dest_x = dest_x,
      .dest_y = dest_y,
      .orig = cells[i].orig,
      .dest = cells[i].dest,
      .elapsed_time = 0
    };
    ++move_len;
  }
}

void insert_append(const board_cell cells[],
                   const dim len) {
  for (dim i = 0; i < len; ++i) {
    const dim index = (insert_first + insert_len)
                      % BUFFER_SIZE;
    insert[index] = (scale_cell) {
      .value = cells[i].value,
      .row = cells[i].row,
      .col = cells[i].col,
      .scale = SCALE_ORIG,
      .elapsed_time = 0
    };
    ++insert_len;
  }
}

void move_update(game_board board,
                 const double delta_time) {
  for (dim i = 0; i < move_len; ++i) {
    const dim index = (move_first + i) % BUFFER_SIZE;
    const double ratio = transition(
      move[index].elapsed_time,
      TRANSLATE_TIME,
      TRANSLATE_EASE_STRENGTH,
      EASE_SIGMOID
    );
    if (move[index].orig_x == move[index].dest_x)
      move[index].y = (
        move[index].orig_y +
        ((int) move[index].dest_y - (int) move[index].orig_y)
        * ratio
      );
    else
      move[index].x = (
        move[index].orig_x +
        ((int) move[index].dest_x - (int) move[index].orig_x)
        * ratio
      );
    move[index].elapsed_time += delta_time;
    board[move[index].orig.row][move[index].orig.col] = 0;
    board[move[index].dest.row][move[index].dest.col] = 0;
  }
}

void insert_update(game_board board,
                   const double delta_time) {
  for (dim i = 0; i < insert_len; ++i) {
    const dim index = (insert_first + i) % BUFFER_SIZE;
    insert[index].scale = transition(
      insert[index].elapsed_time,
      SCALE_TIME,
      SCALE_EASE_STRENGTH,
      EASE_SIGMOID
    );
    insert[index].elapsed_time += delta_time;
    board[insert[index].row][insert[index].col] = 0;
  }
}

void move_render(SDL_Renderer* ren) {
  for (dim i = 0; i < move_len; ++i) {
    const dim index = (move_first + i) % BUFFER_SIZE;
    if (move[index].elapsed_time > SCALE_TIME) {
      move_first = (move_first + 1) % BUFFER_SIZE;
      --move_len;
    } else
      draw_cell_pos(
        ren,
        move[index].value,
        move[index].x,
        move[index].y,
        1
      );
  }
}

void insert_render(SDL_Renderer* ren) {
  for (dim i = 0; i < insert_len; ++i) {
    const dim index = (insert_first + i) % BUFFER_SIZE;
    if (insert[index].elapsed_time > SCALE_TIME) {
      insert_first = (insert_first + 1) % BUFFER_SIZE;
      --insert_len;
    } else
      draw_cell(
        ren,
        insert[index].value,
        insert[index].row,
        insert[index].col,
        insert[index].scale
      );
  }
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

void move_merge() {
  bool joined[BUFFER_SIZE] = {false};
  for (dim i = 0; i < move_len; ++i)
    if (!joined[i])
      for (dim j = 0; j < move_len; ++j)
        if (
          !joined[j] &&
          move[i].orig.value == move[j].orig.value && 
          ((
            abs(move[i].dest.row - move[j].orig.row) == 1 &&
            move[i].dest.col - move[j].orig.col == 0
          ) || (
            abs(move[i].dest.col - move[j].orig.col) == 1 &&
            move[i].dest.row - move[j].orig.row == 0
          ))
        ) {
          move[j].x = move[i].x;
          move[j].y = move[i].y;
          move[j].orig_x = move[i].orig_x;
          move[j].orig_y = move[i].orig_y;
          move[j].orig.row = move[i].orig.row;
          move[j].orig.col = move[i].orig.col;
          joined[i] = true;
        }
  translation_cell merged[BUFFER_SIZE];
  dim len = 0;
  for (dim i = 0; i < move_len; ++i)
    if (!joined[i]) {
      merged[len] = move[i];
      ++len;
    }
  for (dim i = 0; i < len; ++i)
    move[i] = merged[i];
  move_first = 0;
  move_len = len;
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
  
  move_append(change->move, change->move_len);
  if (change->move_len > 0)
    move_merge();
  // insert_append(change->insert, change->insert_len);
  delta_clear(change);

  game_board board_render;
  board_unchanged(board, board_old, board_render);
  
  move_update(board_render, delta_time);
  insert_update(board_render, delta_time);

  draw_cells(ren, board_render, 1);

  move_render(ren);
  insert_render(ren);

  if (move_len > 0) {
    sprintf(fps, "%5i", move[0].y);
    draw_text(
      ren, fps, BOARD_SIZE / 2, HEADER_SIZE / 2 + 30,
      0, 30, (SDL_Color){0,0,0}, ALIGN_LEFT
    );
  }
  
  return move_len > 0 || insert_len > 0;
}
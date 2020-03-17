#ifndef HEADER_LOGIC
  #define HEADER_LOGIC
  #include "2048.h"
#endif

#ifndef HEADER_DRAW
  #define HEADER_DRAW
  #include "draw.h"
#endif

#include "buttons.c"

const SDL_Color cell_colors[] = {
  {201, 187, 178, 255},
  {239, 229, 216, 255},
  {236, 224, 201, 255},
  {243, 177, 120, 255},
  {244, 149, 99,  255},
  {247, 124, 93,  255},
  {246, 93,  59,  255},
  {237, 206, 113, 255},
  {117, 167, 241, 255},
  {69,  133, 242, 255},
  {237, 207, 114, 255},
  {237, 197, 63,  255}
};

TTF_Font* font = NULL;

void inline font_open() {
  if (font == NULL)
    font = TTF_OpenFont(FONT_NAME, FONT_PT_SIZE);
}

void inline font_close() {
  if (font != NULL)
    TTF_CloseFont(font);
}

void color_add(SDL_Color* base, const SDL_Color delta) {
  if (base->r + delta.r < UINT8_MAX)
    base->r += delta.r;
  else
    base->r = UINT8_MAX;
  if (base->g + delta.g < UINT8_MAX)
    base->g += delta.g;
  else
    base->g = UINT8_MAX;
  if (base->b + delta.b < UINT8_MAX)
    base->b += delta.b;
  else
    base->b = UINT8_MAX;
  if (base->a + delta.a < UINT8_MAX)
    base->a += delta.a;
  else
    base->a = UINT8_MAX;
}

void color_sub(SDL_Color* base, const SDL_Color delta) {
  if (base->r - delta.r >= 0)
    base->r -= delta.r;
  else
    base->r = 0;
  if (base->g - delta.g >= 0)
    base->g -= delta.g;
  else
    base->g = 0;
  if (base->b - delta.b >= 0)
    base->b -= delta.b;
  else
    base->b = 0;
  if (base->a - delta.a >= 0)
    base->a -= delta.a;
  else
    base->a = 0;
}

void color_foreground(const SDL_Color bg, SDL_Color* fg) {
  if (bg.r + bg.g + bg.b > 540) {
    fg->r = 119;
    fg->g = 110;
    fg->b = 101;
  } else {
    fg->r = 249;
    fg->g = 246;
    fg->b = 242;
  }
  fg->a = 255;
}

text_raster* text_write(SDL_Renderer *ren,
                        const char text[], const SDL_Color color) {
  font_open();
  text_raster* new_raster = (text_raster*) malloc(sizeof(text_raster));
  SDL_Surface* surface =  TTF_RenderText_Blended(font, text, color);
  new_raster->texture = SDL_CreateTextureFromSurface(ren, surface);
  SDL_FreeSurface(surface);
  TTF_SizeText(font, text, &new_raster->width, &new_raster->height);
  return new_raster;
}

void inline text_destroy(text_raster* raster) {
  SDL_DestroyTexture(raster->texture);
  free(raster);
}

void draw_text(SDL_Renderer *ren, const char text[],
               const pos x, const pos y,
               const pos max_w, const pos max_h,
               const SDL_Color color, const text_align align) {
  text_raster* raster = text_write(ren, text, color);
  SDL_Rect rect = (SDL_Rect){
    .x = x, .y = y, .w = raster->width, .h = raster->height
  };
  if (
    max_w != 0 && rect.w > max_w
  ) {
    rect.h *= max_w / (double)rect.w;
    rect.w = max_w;
  } 
  if (max_h != 0 && rect.h > max_h) {
    rect.w *= max_h / (double)rect.h;
    rect.h = max_h;
  }
  if (max_h != 0)
    rect.y = y + (max_h - rect.h) / 2.0;
  if (align == ALIGN_RIGHT) {
    rect.x = x + max_w - rect.w;
  } else if (align == ALIGN_MIDDLE) {
    rect.x = x + (max_w - rect.w) / 2.0;
  }
  SDL_RenderCopy(ren, raster->texture, NULL, &rect);
  text_destroy(raster);
}

void draw_button(SDL_Renderer *ren,
                const button btn, const button_state state) {
  SDL_Color fg,
            bg_true = (SDL_Color) {
              btn.color.r,
              btn.color.g,
              btn.color.b,
              btn.color.a
            };
  if (state == BTN_HOVERED)
    color_add(
      &bg_true,
      (SDL_Color) {
        BTN_COLOR_DELTA,
        BTN_COLOR_DELTA,
        BTN_COLOR_DELTA,
        0
      }
    );
  else if (state == BTN_ACTIVE)
    color_sub(
      &bg_true,
      (SDL_Color) {
        BTN_COLOR_DELTA,
        BTN_COLOR_DELTA,
        BTN_COLOR_DELTA,
        0
      }
    );
  color_foreground(btn.color, &fg);
  roundedBoxRGBA(
    ren, btn.x, btn.y,
    btn.x + btn.w, btn.y + btn.h, BTN_BORDER_RAD,
    bg_true.r, bg_true.g, bg_true.b,
    (bg_true.a > 0) ? bg_true.a : UINT8_MAX
  );
  draw_text(
    ren, btn.name,
    btn.x + BTN_PADDING, btn.y + BTN_PADDING,
    btn.w - 2 * BTN_PADDING, btn.h - 2 * BTN_PADDING,
    fg, btn.align
  );
}

void draw_cell_text(SDL_Renderer *ren, const char text[],
                    const pos x, const pos y,
                    const SDL_Color bg) {
  SDL_Color fg;
  color_foreground(bg, &fg);
  draw_text(
    ren, text, x + CELL_PADDING, y + CELL_PADDING,
    CELL_SIZE - 2 * CELL_PADDING,
    CELL_SIZE - 2 * CELL_PADDING,
    fg, ALIGN_MIDDLE
  );
}

void draw_cell_box(SDL_Renderer *ren, const SDL_Color color,
                   const pos x, const pos y) {
  roundedBoxRGBA(
    ren, x, y, x + CELL_SIZE, y + CELL_SIZE, CELL_BORDER_RAD,
    color.r, color.g, color.b,
    255
  );
}

void draw_cell(SDL_Renderer *ren, const val value,
               const dim row, const dim col) {
  const val ind = log2(value);
  const pos x = BOARD_PADDING + 
            (BOARD_PADDING + CELL_SIZE) * col;
  const pos y = HEADER_SIZE + BOARD_PADDING + 
            (BOARD_PADDING + CELL_SIZE) * row;
  draw_cell_box(ren, cell_colors[ind], x, y);
  if (value != 0) {
    static char value_string[8];
    sprintf(value_string, "%i", value);
    draw_cell_text(ren, value_string, x, y, cell_colors[ind]);
  }
}

void draw_header(SDL_Renderer *ren,
                 const game_state state, const val score) {
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
  SDL_Rect header_rect = {
		0, 0, BOARD_SIZE, HEADER_SIZE
	};
  SDL_RenderFillRect(ren, &header_rect);
  static char score_string[32];
  sprintf(score_string, "Score: %24i", score);
  draw_text(
    ren, score_string, BOARD_PADDING, BOARD_PADDING,
    BOARD_SIZE / 2 - BOARD_PADDING, 28,
    (SDL_Color) {119, 110, 101}, ALIGN_LEFT
  );
  static char state_string[32];
  switch (state) {
    case LOST:
      sprintf(state_string, "Lost");
      break;
    case PLAYING:
      sprintf(state_string, "Playing");
      break;
    case WON:
      sprintf(state_string, "Won");
      break;
    case OUT_OF_MOVES:
      sprintf(state_string, "Out of moves");
      break;
  }
  draw_text(
    ren, state_string,
    BOARD_PADDING, BOARD_PADDING + 48,
    BOARD_SIZE / 2 - BOARD_PADDING, 28,
    (SDL_Color) {119, 110, 101}, ALIGN_LEFT
  );
}

void draw_buttons(SDL_Renderer *ren,
                  pos mouse_x, pos mouse_y, bool mouse_down) {

  for (Uint8 b = 0; b < sizeof(buttons) / sizeof(button); ++b) {
    button_state btn_state = BTN_NORMAL;
    if (
      mouse_x >= buttons[b].x && mouse_x <= buttons[b].x + buttons[b].w &&
      mouse_y >= buttons[b].y && mouse_y <= buttons[b].y + buttons[b].h
    ) {
      if (mouse_down)
        btn_state = BTN_ACTIVE;
      else
        btn_state = BTN_HOVERED;
    }
    draw_button(ren, buttons[b], btn_state);
    if (btn_state == BTN_ACTIVE)
      button_click(buttons[b].action);
    // draw_text(ren, buttons[b].name, 10, 10, 0, 20, (SDL_Color){0,0,0}, ALIGN_NONE);
  }
}

void draw_board(SDL_Renderer *ren, game_board board) {
  SDL_SetRenderDrawColor(ren, 187,173,160, 255);
  SDL_Rect board_rect = {
		0, HEADER_SIZE, BOARD_SIZE, BOARD_SIZE
	};
  SDL_RenderFillRect(ren, &board_rect);
  for (dim r = 0; r < BOARD_DIM; ++r)
    for (dim c = 0; c < BOARD_DIM; ++c)
      draw_cell(ren, board[r][c], r, c);
}
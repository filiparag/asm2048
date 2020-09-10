#include "./draw.h"

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

void font_open(draw_store* store) {
    if (store->font == NULL)
        store->font = TTF_OpenFont(FONT_NAME, FONT_PT_SIZE);
}

void font_close(draw_store* store) {
    if (store->font != NULL)
        TTF_CloseFont(store->font);
}

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, time_store* time) {
    store->render = render;
    store->font = NULL;
    store->game = game;
    store->time = time;
}

void draw_rectangle(draw_store* store, const pix x, const pix y, const pix w, const pix h, const pix r, const SDL_Color c) {
    roundedBoxRGBA(
        store->render, x, y,
        x + w, y + h, r,
        c.r, c.g, c.b,
        255
    );
}

void draw_background(draw_store* store) {
  SDL_SetRenderDrawColor(store->render, 250,248,239, 255);
  SDL_Rect board_rect = {
		0, 0, BOARD_SIZE, HEADER_SIZE + BOARD_SIZE
	};
  SDL_RenderFillRect(store->render, &board_rect);
}

text_raster* text_write(draw_store* store,
                        const char text[], const SDL_Color color) {
    text_raster* new_raster = (text_raster*) malloc(sizeof(text_raster));
    font_open(store);
    SDL_Surface* surface =  TTF_RenderText_Blended(store->font, text, color);
    new_raster->texture = SDL_CreateTextureFromSurface(store->render, surface);
    SDL_FreeSurface(surface);
    TTF_SizeText(store->font, text, &new_raster->width, &new_raster->height);
    return new_raster;
}

void text_destroy(text_raster* raster) {
  SDL_DestroyTexture(raster->texture);
  free(raster);
}

void draw_text(draw_store* store, const char text[],
               const pix x, const pix y,
               const pix max_w, const pix max_h,
               const SDL_Color color, const text_align align) {
    text_raster* raster = text_write(store, text, color);
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
    SDL_RenderCopy(store->render, raster->texture, NULL, &rect);
    text_destroy(raster);
}

void draw_cell_text(draw_store* store, const char text[],
                    const pix x, const pix y, const pix size,
                    const pix padding, const SDL_Color bg) {
  SDL_Color fg;
  color_foreground(bg, &fg);
  draw_text(
    store, text, x + padding, y + padding,
    size - 2 * padding,
    size - 2 * padding,
    fg, ALIGN_MIDDLE
  );
}

void draw_cell_box(draw_store* store, const SDL_Color color,
                   const pix x, const pix y, const pix size) {
  roundedBoxRGBA(
    store->render, x, y,
    x + size, y + size,
    CELL_BORDER_RAD,
    color.r, color.g, color.b,
    255
  );
}

void draw_cell_pix(draw_store* store, const pix x, const pix y, const double scale) {
    dim row, col;
    pix_to_dim(x, y, &row, &col);
    val value = store->game->board[row][col];
    const val ind = log2(value);
    const pix size = CELL_SIZE * scale,
                padding = CELL_PADDING * scale;
    const double dsize = CELL_SIZE - CELL_SIZE * scale;
    const pix x_scaled = x + dsize / 2,
                y_scaled = y + dsize / 2;
    draw_cell_box(
        store, cell_colors[ind], x_scaled, y_scaled, size
    );
    if (value != 0) {
        static char value_string[10];
        sprintf(value_string, "%u", value);
        draw_cell_text(
            store, value_string, x_scaled, y_scaled,
            size, padding, cell_colors[ind]
        );
    }
}

void draw_cell(draw_store* store, const dim row, const dim col, const double scale) {
    pix x, y;
    dim_to_pix(row, col, &x, &y);
    draw_cell_pix(store, x, y, scale);
}

// void draw_cell_pix(draw_store* store, const pix x, const pix y) {
//     dim row, col;
//     pix_to_dim(x, y, &row, &col);
//     val value = store->game->board[row][col];
//     SDL_Color color = cell_colors[(int) log2(value+1)];
//     draw_rectangle(store, x, y, CELL_SIZE, CELL_SIZE, CELL_BORDER_RAD, color);
// }

// void draw_cell(draw_store* store, const dim row, const dim col) {
//     pix x, y;
//     dim_to_pix(row, col, &x, &y);
//     draw_cell_pix(store, x, y);
// }

void draw(draw_store* store) {
    draw_background(store);
    draw_rectangle(store, 0, HEADER_SIZE, BOARD_SIZE, BOARD_SIZE, 0, (SDL_Color) {187,173,160});
    for (dim i = 0; i < BOARD_DIM; ++i)
         for (dim j = 0; j < BOARD_DIM; ++j)
            draw_cell(store, i, j, 1);
}
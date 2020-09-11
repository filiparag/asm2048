#include "./draw.h"

void font_open(draw_store* store) {
    if (store->font == NULL)
        store->font = TTF_OpenFont(FONT_NAME, FONT_PT_SIZE);
}

void font_close(draw_store* store) {
    if (store->font != NULL)
        TTF_CloseFont(store->font);
}

void image_load(draw_store* store) {
    SDL_Surface *image;
    image = SDL_LoadBMP(IMAGE_NEW);
    store->buttons[(button_name) NEW].image = SDL_CreateTextureFromSurface(store->render, image);
    SDL_FreeSurface(image);
    image = SDL_LoadBMP(IMAGE_UNDO);
    store->buttons[(button_name) UNDO].image = SDL_CreateTextureFromSurface(store->render, image);
    SDL_FreeSurface(image);
    // image = SDL_LoadBMP(IMAGE_SHARE);
    // store->buttons[(button_name) SHARE].image = SDL_CreateTextureFromSurface(store->render, image);
    // SDL_FreeSurface(image);
}

void image_unload(draw_store* store) {
    SDL_DestroyTexture(store->buttons[(button_name) NEW].image);
    SDL_DestroyTexture(store->buttons[(button_name) UNDO].image);
    // SDL_DestroyTexture(store->buttons[(button_name) SHARE].image);
}

SDL_Color color(const val value) {
    SDL_Color colors[] = {
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
    if (value == 0)
        return colors[0];
    else if (value <= 2048) {
        const val i = log2(value);
        return colors[i];
    } else
        return (SDL_Color) {0, 0, 0};
}

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, input_pointer* mouse, time_store* time) {
    store->render = render;
    store->font = NULL;
    font_open(store);
    store->game = game;
    store->mouse = mouse;
    store->time = time;
    // store->buttons[SHARE] = (button) {
    //     .x = BOARD_SIZE - 3 * BTN_SIZE - BOARD_PADDING - 2 * CELL_PADDING,
    //     .y = HEADER_SIZE - 50 - BOARD_PADDING,
    //     .size = 50
    // };
    store->buttons[UNDO] = (button) {
        .visible = false,
        .x = BOARD_SIZE - 2 * BTN_SIZE - BOARD_PADDING - CELL_PADDING,
        .y = HEADER_SIZE - 50 - BOARD_PADDING,
        .size = 50
    };
    store->buttons[NEW] = (button) {
        .visible = true,
        .x = BOARD_SIZE - BTN_SIZE - BOARD_PADDING,
        .y = HEADER_SIZE - 50 - BOARD_PADDING,
        .size = 50
    };
    image_load(store);
}

void draw_close(draw_store* store) {
    font_close(store);
    image_unload(store);
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

text_raster* text_write(draw_store* store, const char text[], const SDL_Color color) {
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
    const pix size = CELL_SIZE * scale,
                padding = CELL_PADDING * scale;
    const double dsize = CELL_SIZE - CELL_SIZE * scale;
    const pix x_scaled = x + dsize / 2,
                y_scaled = y + dsize / 2;
    draw_cell_box(
        store, color(value), x_scaled, y_scaled, size
    );
    if (value != 0) {
        static char cell_value_string[10];
        sprintf(cell_value_string, "%u", value);
        draw_cell_text(
            store, cell_value_string, x_scaled, y_scaled,
            size, padding, color(value)
        );
    }
}

void draw_button(draw_store* store, const button_name btn) {
    if (!store->buttons[btn].visible)
        return;
    pix x = store->buttons[btn].x,
        y = store->buttons[btn].y,
        s = store->buttons[btn].size;
    SDL_Rect dstrect = {x + s * 0.2, y + s * 0.2, s * 0.6, s * 0.6};
    SDL_Color color = {143,122,102};
    if (
        store->mouse->x >= x && store->mouse->x <= x + s &&
        store->mouse->y >= y && store->mouse->y <= y + s
    ) {
        SDL_Cursor* cursor;
        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        SDL_SetCursor(cursor);
        if (store->mouse->lmb)
            color = (SDL_Color) {85,60,37};
        else
            color = (SDL_Color) {94,78,63};
    }
    draw_rectangle(store, x, y, s, s, s * 0.1, color);
    SDL_RenderCopy(store->render, store->buttons[btn].image, NULL, &dstrect);
}

void draw_cell(draw_store* store, const dim row, const dim col, const double scale) {
    pix x, y;
    dim_to_pix(row, col, &x, &y);
    draw_cell_pix(store, x, y, scale);
}

void draw(draw_store* store) {
    draw_background(store);
    draw_rectangle(store, 0, HEADER_SIZE, BOARD_SIZE, BOARD_SIZE, 0, (SDL_Color) {187,173,160});
    for (dim i = 0; i < BOARD_DIM; ++i)
         for (dim j = 0; j < BOARD_DIM; ++j)
            draw_cell(store, i, j, 1);
    static char score_string[48];
    sprintf(score_string, "Score: %u", store->game->score);
    draw_text(
        store, score_string, BOARD_PADDING, BOARD_PADDING - 5,
        BOARD_SIZE - 3 * BTN_SIZE - 2 * BOARD_PADDING - 2 * CELL_PADDING, 40,
        (SDL_Color) {119, 110, 101}, ALIGN_LEFT
    );
    static char state_string[48];
    switch (store->game->state) {
        case PLAYING:
            sprintf(state_string, "Playing 2048");
            break;
        case WON:
            sprintf(state_string, "Winning 2048");
            break;
        case LOST:
            sprintf(state_string, "You lost!");
            break;
        case WON_END:
            sprintf(state_string, "You won!");
            break;
        case QUIT:
            break;
    }
    draw_text(
        store, state_string, BOARD_PADDING, BOARD_PADDING + 33,
        BOARD_SIZE - 3 * BTN_SIZE - 2 * BOARD_PADDING - 2 * CELL_PADDING, 22,
        (SDL_Color) {119, 110, 101}, ALIGN_LEFT
    );
    SDL_Cursor* cursor;
    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_SetCursor(cursor);
    for (dim i = 0; i < BTN_COUNT; ++i)
        draw_button(store, i);

}
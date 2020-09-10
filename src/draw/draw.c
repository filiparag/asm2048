#include "./draw.h"

void draw_initialize(draw_store* store, SDL_Renderer* render, game_store* game, time_store* time) {
    store->render = render;
    store->game = game;
    store->time = time;
}

void draw_rectangle(draw_store* store, const pixel x, const pixel y, const pixel w, const pixel h, const pixel r, const SDL_Color c) {
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

void draw(draw_store* store) {
    draw_background(store);
    draw_rectangle(store, 0, HEADER_SIZE, BOARD_SIZE, BOARD_SIZE, 0, (SDL_Color) {187,173,160});
}
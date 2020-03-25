#include "window.h"

bool window_init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return false;
	}
	win = SDL_CreateWindow(
		"2048",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		BOARD_SIZE,
		BOARD_SIZE + HEADER_SIZE,
		SDL_WINDOW_SHOWN
	);
	if (win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return false;
	}
	ren = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (ren == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		if (win != NULL) {
			SDL_DestroyWindow(win);
		}
		SDL_Quit();
		return false;
	}
	if (TTF_Init() < 0) {
		SDL_Quit();
		return false;
	}
  SDL_RenderClear(ren);
	return true;
}

void window_render() {
  SDL_RenderPresent(ren);
  SDL_RenderClear(ren);
}

void window_close() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	font_close();
	atexit(TTF_Quit);
}
#include "./window.h"

bool window_init(window_store* store) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return false;
	}
	store->window = SDL_CreateWindow(
		"asm2048",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		BOARD_SIZE,
		BOARD_SIZE + HEADER_SIZE,
		SDL_WINDOW_SHOWN
	);
	if (store->window == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return false;
	}
	store->render = SDL_CreateRenderer(
		store->window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (store->render == NULL) {
		fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
		if (store->window != NULL) {
			SDL_DestroyWindow(store->window);
		}
		SDL_Quit();
		return false;
	}
    SDL_RenderClear(store->render);
    draw_initialize(&store->draw, store->render, &store->game, &store->control.mouse, &store->time);
	if (TTF_Init() < 0) {
		SDL_Quit();
		return false;
	}
    game_initialize(&store->game, BOARD_DIM_X, BOARD_DIM_Y);
    time_initialize(&store->time);
    control_initialize(store);
    return true;
}

void window_new_frame(window_store* store) {
    SDL_RenderPresent(store->render);
    SDL_RenderClear(store->render);
    time_update(&store->time);
    control_read_events(store);
    while(control_read_events(store));
    draw(&store->draw);
}

void window_close(window_store* store) {
    SDL_DestroyRenderer(store->render);
	SDL_DestroyWindow(store->window);
	SDL_Quit();
	draw_close(&store->draw);
	atexit(TTF_Quit);
}
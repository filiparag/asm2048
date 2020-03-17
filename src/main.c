#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "2048.c"
#include "draw.c"

#define NO_STDIO_REDIRECT

int main(int argc, char *argv[])
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Window *win = SDL_CreateWindow(
		"2048",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		BOARD_SIZE,
		BOARD_SIZE + HEADER_SIZE,
		SDL_WINDOW_SHOWN
	);
	if (win == NULL) {
		fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(
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
		return EXIT_FAILURE;
	}

	if (TTF_Init() < 0) {
		SDL_Quit();
		return EXIT_FAILURE;
	}

	srand(time(NULL));
  game_store game = {LOST};
  game_initialize(&game);

	SDL_Event event;
	bool quit = false;
	bool mouse_down = false;

	while (!quit) {
		int mouse_x, mouse_y;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym ){
						case SDLK_LEFT:
							game_action(&game, LEFT);
							break;
						case SDLK_RIGHT:
							game_action(&game, RIGHT);
							break;
						case SDLK_UP:
							game_action(&game, UP);
							break;
						case SDLK_DOWN:
							game_action(&game, DOWN);
							break;
						case SDLK_n:
							game_initialize(&game);
							break;
						case SDLK_q:
							quit = true;
							break;
						case SDLK_d:
							game.board[0][0] *= 2;
							break;
						case SDLK_f:
							board_insert(game.board);
							break;
						default:
								break;
					}
				case SDL_KEYUP:
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_down = true;
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
						mouse_down = false;
					break;
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break;
			}
		}
		if (!mouse_down)
			button_handle_clicks(&game);
		SDL_RenderClear(ren);
		SDL_GetMouseState(&mouse_x, &mouse_y);
		draw_header(ren, game.state, game.score);
		draw_board(ren, game.board);
		draw_buttons(ren, mouse_x, mouse_y, mouse_down);
		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	font_close();
	atexit(TTF_Quit);

	return 0;
}
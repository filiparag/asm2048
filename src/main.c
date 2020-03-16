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

	while (!quit) {
		while( SDL_PollEvent( &event ) ){
			SDL_RenderClear(ren);
			switch( event.type ){
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

				case SDL_QUIT:
						quit = true;
						break;

				default:
						break;
			}
			draw_header(ren, game.state, game.score);
			draw_board(ren, game.board);
			SDL_RenderPresent(ren);
			SDL_Delay(1);
		}
		
		// draw_cell(ren, 1, 0, 1);
		// draw_cell(ren, 1, 0, 2);
		// draw_cell(ren, 1, 0, 3);
		// draw_cell(ren, 4, 0, 0);
		// draw_cell(ren, 8, 1, 1);
		// draw_cell(ren, 16, 2, 1);
		// draw_cell(ren, 2, 1, 2);
		// draw_cell(ren, 32, 3, 2);
		// draw_cell(ren, 128, 3, 3);
		
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	atexit(TTF_Quit);

	return 0;
}
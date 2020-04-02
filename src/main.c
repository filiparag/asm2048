#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "window/window.h"
#include "game/types.h"
#include "game/2048.h"
#include "control/game.h"
#include "control/buttons.h"
#include "render/draw.h"
#include "render/animate.h"

int main(int argc, char *argv[]) {

	if (!window_init())
		return EXIT_FAILURE;
 
	game_init();

	while (!game_end()) {

		game_tick(!animate_in_progress());
		
		game_render(ren);

		// cheats
		dim rm, cm;
		pos_to_dim(mouse.x, mouse.y, &rm, &cm);
		if (rm < BOARD_DIM && cm < BOARD_DIM) {
			debug(ren, rm, 1);
			debug(ren, cm, 2);
		}

		if (mouse.lmb) {
			if (game.board[rm][cm] == 0)
				game.board[rm][cm] = 2;
			else
				game.board[rm][cm] *= 2;
			SDL_Delay(100);
		}

		if (mouse.rmb) {
			if (game.board[rm][cm] == 2)
				game.board[rm][cm] = 0;
			else
				game.board[rm][cm] /= 2;
			SDL_Delay(100);
		}
		// cheats

		window_render();
	}

	window_close();

	return 0;
}
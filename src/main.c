#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "window/window.h"
#include "game/types.h"
#include "game/2048.h"
#include "control/game.h"
#include "render/draw.h"
#include "render/buttons.h"
#include "render/animate.h"

int main(int argc, char *argv[]) {
	
	// game_play_console();
	// return 0;

	if (!window_init())
		return EXIT_FAILURE;

	srand(time(NULL));
  game_store game = {STATE_LOST};
  game_initialize(&game);
	game_init();

	game_board old_board;
	for (dim r = 0; r < BOARD_DIM; ++r)
		for (dim c = 0; c < BOARD_DIM; ++c)
			old_board[r][c] = 0;

	while (game.state != STATE_QUIT) {

		// if (!mouse_down) {
		// 	button_handle_clicks(&game);
		// } else {
		// 	dim r, c;
		// 	pos_to_dim(mouse_x, mouse_y, &r, &c);
		// 	if (r < BOARD_DIM && c < BOARD_DIM) {
		// 		if (game.board[r][c] == 0)
		// 			game.board[r][c] = 2;
		// 		else
		// 			game.board[r][c] *= 2;
		// 	}
		// 	SDL_Delay(100);
		// }
		SDL_RenderClear(ren);
		draw_header(ren, game.state, game.score);
		draw_board(ren);
		draw_buttons(ren, mouse.x, mouse.y, mouse.lmb);
		
		if (!animate_board(ren, game_time.delta, game.board, old_board, &game.delta)) {
			draw_cells(ren, game.board, 1);
		}

		window_render();
	}

	window_close();

	return 0;
}
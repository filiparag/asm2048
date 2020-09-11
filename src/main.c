#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "game/game.h"
#include "window/window.h"

int main(int argc, char *argv[]) {

	srand(time(0));

	window_store store;

	if (!window_init(&store))
		return EXIT_FAILURE;

	while (store.game.state != QUIT) {
		window_new_frame(&store);
		#ifndef NDEBUG
		system("clear");
		printf("FPS:   %u\n", store.time.fps);
		game_print(&store.game);
		#endif
	}

	window_close(&store);

	return 0;
}
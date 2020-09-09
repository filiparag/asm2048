#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>

#include "game/game.h"

int main(int argc, char *argv[]) {

	srand(time(0));

	game_store store;

	game_initialize(&store, 4, 4);

	game_board_insert(store.board, 4, 4);

	game_print(&store);

	return 0;
}
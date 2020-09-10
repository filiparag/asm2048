#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "game/game.h"

int main(int argc, char *argv[]) {

	srand(time(0));

	game_store store;

	game_initialize(&store, 4, 4);

	printf("\033[H\033[J");
	game_board_insert_random(store.board, 4, 4);
	game_print(&store);
	
	game_move move;
	int c;
	/* use system call to make terminal send all keystrokes directly to stdin */
	system ("/bin/stty raw");
	while((c=getchar())!= 'q') {
		/* type a period to break out of the loop, since CTRL-D won't work raw */
		// putchar(c);
		if (c == '\033') { // if the first value is esc
			getchar(); // skip the [
			switch(getchar()) { // the real value
				case 'A':
					// code for arrow up
					printf("UP");
					move = UP;
					break;
				case 'B':
					// code for arrow down
					printf("DOWN");
					move = DOWN;
					break;
				case 'C':
					// code for arrow right
					printf("RIGHT");
					move = RIGHT;
					break;
				case 'D':
					// code for arrow left
					printf("LEFT");
					move = LEFT;
					break;
			}
			printf("\033[H\033[J");
			system ("/bin/stty cooked");
			game_make_move(&store, move);
			game_print(&store);
			system ("/bin/stty raw");
		}
	}
	/* use system call to set terminal behaviour to more normal behaviour */
	system ("/bin/stty cooked");

	return 0;
}
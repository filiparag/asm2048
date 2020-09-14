#include <stdint.h>

#include "../constants.h"

void game_board_clear_asm(uint32_t board[DIM_MAX][DIM_MAX], int8_t rows, int8_t cols);
bool game_board_out_of_moves_asm(uint32_t board[DIM_MAX][DIM_MAX], int8_t rows, int8_t cols);
bool game_board_move_next_asm(const game_move move, const board_move* m, dim* i, dim* j);
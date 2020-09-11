#include <stdint.h>

#include "./constants.h"

void game_board_clear_asm(uint32_t board[DIM_MAX][DIM_MAX], int8_t rows, int8_t cols);
#include "../constants.h"

.global game_board_clear_asm

game_board_clear_asm:
        movq    $DIM_MAX*DIM_MAX, %r11

iterate:
        decq    %r11
        movl    $0, (%rdi, %r11, 4)
        cmpq    $0, %r11
        jz      end
        jmp     iterate

end:
        ret
#include "constants.h"

.global board_clear_asm

dim = BOARD_DIM

board_clear_asm:

  movq    %rdi, %rax          // board matrix address to %rax
  movq    $dim*dim,  %rcx     // number of cells

  zero_cell:

    movw    $0, (%rax)        // set current cell to 0
    addq    $2,  %rax         // move to next cell

    decq    %rcx              // count down remaining cells
    cmpq    $0, %rcx
    ja zero_cell

  ret
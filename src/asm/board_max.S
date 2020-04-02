#include "constants.h"

.global board_max_asm

dim = BOARD_DIM

board_max_asm:

  movq    %rdi, %rdx          // board matrix address to %rax
  movq    $dim*dim, %rcx      // number of cells
  movq    $0, %rax            // maximum value

  compare_cell:

    cmpw    (%rdx), %ax       // compare current cell and maximum
    ja      next_cell
    movw    (%rdx), %ax       // set maximum to current

  next_cell:

    addq    $2, %rdx          // move to next cell

    decq    %rcx              // count down remaining cells
    cmpq    $0, %rcx
    ja      compare_cell

  ret
    

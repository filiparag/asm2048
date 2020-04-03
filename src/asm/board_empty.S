#include "constants.h"

.global board_empty_asm

dim = BOARD_DIM

board_empty_asm:

  pushq   %rbx

  movq    %rsi, %rbx          // empty cell array address to %rbx
  movq    %rdi, %rdx          // board matrix address to %rdx

  movq    $0, %rax            // empty cell counter

  movb    $0, %ch             // current column
  movb    $0, %cl             // current row

  check_empty:
  cmpw    $0, (%rdx)          // check if current board cell is empty
  je      found_empty
  jmp     next_cell

  found_empty:
  movb    %cl, 1(%rbx)         // set current row for new empty cell
  movb    %ch, (%rbx)          // set current column for new empty cell

  addq    $4, %rbx            // move to next empty element
  incq    %rax                // increment empty cell counter

  next_cell:
  cmpb    $dim-1, %cl         // check if on last column of current row
  je      next_row
  incb    %cl                 // increment current column
  addq    $2, %rdx            // move to next cell for next iteration
  jmp     check_empty

  next_row:
  movb    $0, %cl             // set columns to BOARD_DIM for new row
  cmpb    $dim-1, %ch         // check if on last row
  je      done
  incb    %ch                 // increment current row
  addq    $2, %rdx            // move to next cell for next iteration
  jmp     check_empty
  
  jmp     check_empty

  done:
  popq    %rbx

  ret
#include "constants.h"

.global board_add_asm

dim = BOARD_DIM

board_add_asm:

  pushq   %rbp
  movq    %rsp, %rbp
  pushq   %rbx

  movq    %rdi, -8(%rbp)    // function parameter: game_board board
  movl    %esi, -12(%rbp)   // function parameter: const direction dir
  movq    %rdx, -24(%rbp)   // function parameter: board_delta* delta
  subq    $32,  %rsp        // pad stack pointer by parameters size

  movw    $0, -32(%rsp)     // local valiable: score
  subq    $2, %rsp          // pad stack pointer by variable size

  movl    -12(%rbp), %edi   // call parameter: direction
  movl    $1, %esi          // call parameter: border
  call    index_first
  movb    %al, -34(%rbp)    // local constant: first
  subq    $1, %rsp          // pad stack pointer by constant size

  movl    -12(%rbp), %edi   // call parameter: direction
  movl    $1, %esi          // call parameter: border
  call    index_last
  movb    %al, -35(%rbp)    // local constant: last
  subq    $1, %rsp
 
  movl    -12(%rbp), %edi   // call parameter: direction
  call    index_next
  movb    %al, -36(%rbp)    // local constant: next
  subq    $1, %rsp
  
  movb    $0, %ch           // iterator a
  movb    -34(%rbp), %cl    // iterator b

  jmp loop

  outer_loop:
    cmpb    $dim, %ch       // a < BOARD_DIM
    je      done
    movb    -34(%rbp), %cl  // next inner loop
    incb    %ch

  inner_loop:
    cmpb  %cl, -35(%rbp)    // b != last
    je    outer_loop
    addb  -36(%rbp), %cl

  loop:
    subq    $4, %rsp        // local variables: r, c, r_next, c_next

    pushq   %rcx            // stash loop counter

    movl    -12(%rbp), %edi // call parameter: direction
    xorq    %rax, %rax        // zero junk bits
    movb    %ch, %al
    movq    %rax, %rsi      // call parameter: a
    movb    %cl, %al
    movq    %rax, %rdx      // call parameter: b
    leaq    -37(%rbp), %rcx // call parameter: r
    leaq    -38(%rbp), %r8  // call parameter: c
    leaq    -39(%rbp), %r9  // call parameter: r_next
    leaq    -40(%rbp), %rax 
    pushq   %rax            // call parameter: c_next
    call    index_cell
    addq    $8, %rsp

    
    xorq    %rax, %rax      // current board[r][c]
    movb    -37(%rbp), %al  // row index
    shlq    $2, %rax        // multiply row index by 4
    addb    -38(%rbp), %al  // column index
    shlq    $1, %rax        // double index for word size
    movb    %al, %bl        // store board[r][c] index in %bl

    movq    -8(%rbp), %rcx
    addq    %rax, %rcx
    movw    (%rcx), %ax    // board[r][c] to %ax

    cmpw    $0, %ax        // board[r][c] == 0
    je      continue
    pushw   %ax            // stash value

    xorq    %rax, %rax      // current board[r_next][c_next]
    movb    -39(%rbp), %al  // row_next index
    shlq    $2, %rax        // multiply row index by 4
    addb    -40(%rbp), %al  // column_next index
    shlq    $1, %rax        // double index for word size
    movb    %al, %bh        // store board[r_next][c_next] index in %bh

    movq    -8(%rbp), %rcx
    addq    %rax, %rcx
    movw    (%rcx), %ax     // board[r_next][c_next] to %ax

    popw    %cx             // restore board[r][c] to %cx

    cmpw    %ax, %cx        // board[r][c] == board[r_next][c_next]
    jne     continue


    movq    -24(%rbp), %rdi // call parameter: delta
    movq    -8(%rbp), %rsi  // call parameter: board
    movb    -39(%rbp), %dl  // call parameter: r_next
    movb    -40(%rbp), %cl  // call parameter: c_next
    movb    -40(%rbp), %r8b // call parameter: r
    movb    -40(%rbp), %r9b // call parameter: c
    pushw   $1              // call_parameter: merge
    call    delta_move
    addq    $2, %rsp
    

    xorq    %rax, %rax
    movb    %bl, %al
    jmp done

  continue:
    popq    %rcx            // restore loop counter
    jmp inner_loop

  
  done:
  popq    %rbx
  movq    %rbp, %rsp          // delete local variables
  popq    %rbp
  ret
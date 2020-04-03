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

    movl    -12(%rbp), %edi // call parameter: direction
    movq    $0, %rax        // zero junk bits
    movb    %ch, %al
    movq    %rax, %rsi      // call parameter: a
    movb    %cl, %al
    movq    %rax, %rdx      // call parameter: b
    leaq    -37(%rbp), %rcx // call parameter: r
    leaq    -38(%rbp), %r8  // call parameter: c
    leaq    -39(%rbp), %r9  // call parameter: r_next
    leaq    -40(%rbp), %rax 
    pushq   %rax            // call parameter: c_next
    call    index_next
    addq    $8, %rsp

    movq    -40(%rbp), %rax

  
  done:
  popq    %rbx
  movq    %rbp, %rsp          // delete local variables
  popq    %rbp
  ret
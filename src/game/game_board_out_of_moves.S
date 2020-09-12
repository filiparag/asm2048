#include "./constants.h"

.global game_board_out_of_moves_asm

game_board_out_of_moves_asm:
        pushq   %rbx            // (temp) current el value
        pushq   %rcx
        pushq   %r12
        pushq   %r13            // (temp) next element
        pushq   %r15            // (copy) copy cols because mul overwrites %rdx

        movq    $0, %rcx        // moves
        movq    $0, %r11        // current row
        movq    $0, %r12        // current col
        movq    %rdx, %r15      // cols

iterate:
        movq    $DIM_MAX, %rax  // current element
        mulq    %r12            // multiply DIM_MAX by curent col
        addq    %r11, %rax      // add cur row

check_zero:
        cmpl    $0, (%rdi, %rax, 4)
        jne     check_right
        jmp     move_found

check_right:
        movq    %r12, %r13      // move current col
        incq    %r13            // next el right
        cmpq    %r13, %rsi      // col + 1 < cols
        jb      check_down
        movq    %rax, %r13      // move current el
        incq    %r13            // next el right
        movl    (%rdi, %rax, 4), %ebx // store current el value
        cmpl    %ebx, (%rdi, %r13, 4) // compare stored current el with right el
        je      move_found
        jmp     check_down

check_down:
        movq    %r11, %r13      // move current row
        incq    %r13            // next el right
        cmpq    %r13, %r15      // row + 1 < rows
        jb      next_element
        movq    %rax, %r13      // move current el
        addq    $DIM_MAX, %r13  // add DIM_MAX elements to current (set below)
        cmpl    %ebx, (%rdi, %r13, 4) // compare stored cur el with below el
        je      move_found
        jmp     next_element

move_found:
        incq    %rcx
        jmp     next_element

next_element:
        incq    %r11
        cmpq    %rsi, %r11      // current row == rows
        je      next_row
        jmp     iterate

next_row:
        incq    %r12
        cmpq    %r12, %r15      // current col == cols
        je      end     
        movq    $0, %r11
        jmp     iterate

end:
        xorq    %rax, %rax      // return value set to 0
        cmpq    $0, %rcx        // moves == 0
        sete    %al             // return 1 if moves == 0

        popq    %r15
        popq    %r13
        popq    %r12
        popq    %rcx
        popq    %rbx

        ret
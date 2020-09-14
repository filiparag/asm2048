#include "../constants.h"

.global game_board_move_next_asm

game_board_move_next_asm:
        pushq   %rbx
        xorq    %rbx, %rbx      // (temp) next index

switch:
        cmpq    $0, %rdi
        je      case_left
        cmpq    $1, %rdi
        je      case_right
        cmpq    $2, %rdi
        je      case_up
        cmpq    $3, %rdi
        je      case_down
        jmp     return_true

case_left:
case_right:
        movb    5(%rsi), %bl    // m->col_inc
        addb    (%rcx), %bl     // add j value (col)
        cmpb    %bl, 3(%rsi)    // *j + m->col_inc != m->col_end
        je      return_false
        movb    5(%rsi), %bl    // m->col_inc
        addb    %bl, (%rcx)     // *j += m->col_inc;
        jmp     return_true

case_up:
case_down:
        movb    4(%rsi), %bl    // m->row_inc
        addb    (%rdx), %bl     // add i value (row)
        cmpb    %bl, 1(%rsi)    // *i + m->row_inc != m->row_end
        je      return_false
        movb    4(%rsi), %bl    // m->row_inc
        addb    %bl, (%rdx)     // *i += m->row_inc;
        jmp     return_true

return_false:
        movq    $0, %rax        // return false
        jmp     end

return_true:
        movq    $1, %rax        // return true

end:
        popq    %rbx

        ret
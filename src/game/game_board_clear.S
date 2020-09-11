.global game_board_clear_asm

game_board_clear_asm:
        testb   %sil, %sil
        jle     .L1
        leal    -1(%rsi), %eax
        movzbl  %al, %eax
        leaq    4(%rdi,%rax,4), %rdx
        leaq    0(,%rax,8), %rcx
        subq    %rax, %rcx
        leaq    28(%rdi,%rcx,4), %rdi
        movzbl  %sil, %esi
        negq    %rsi
        leaq    0(,%rsi,4), %rcx
        jmp     .L4
.L7:
        addq    $24, %rdx
        cmpq    %rdi, %rdx
        je      .L1
.L4:
        leaq    (%rcx,%rdx), %rax
.L3:
        movl    $0, (%rax)
        addq    $4, %rax
        cmpq    %rdx, %rax
        jne     .L3
        jmp     .L7
.L1:
        ret
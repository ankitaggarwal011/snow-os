.text
.global keyboard_isr
keyboard_isr:
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
    callq keyboard_intr_handler
    pop %rdi
    pop %rsi
    pop %rbp
    pop %rbx
    pop %rdx
    pop %rcx
    pop %rax
    sti
    iretq
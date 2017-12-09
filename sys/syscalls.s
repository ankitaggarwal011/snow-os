.text
.global syscall_isr
syscall_isr:
    sti
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
    push %rdi
    // assuming arg registers are already loaded
    movq %rsi, %r8
    movq %rdi, %r9
    movq %rcx, %r15
    movq %rdx, %rcx
    movq %r15, %rdx
    // rcx and rdx should be interchanged
    movq %rbx, %rsi
    movq %rax, %rdi
    call handle_syscall
    pop %rdi
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rdi
    pop %rsi
    pop %rbp
    pop %rbx
    pop %rdx
    pop %rcx
    iretq

.global test_syscall
test_syscall:
    movq $0xFAFFFFF1, %rax
    movq $0xFAFFFFF2, %rbx
    movq $0xFAFFFFF3, %rcx
    movq $0xFAFFFFF4, %rdx
    movq $0xFAFFFFF5, %rsi
    movq $0xFAFFFFF6, %rdi
    INT $0x80
    retq
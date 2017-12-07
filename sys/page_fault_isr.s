.text
.global page_fault_isr
page_fault_isr:
    cli
    push %rax
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
    callq page_fault_handler
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
    pop %rax
    sti
    iretq
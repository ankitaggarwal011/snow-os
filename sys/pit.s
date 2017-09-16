EXTERN pit_intr_handler
timer_isr:
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
    call pit_intr_handler
    pop %rdi    
    pop %rsi    
    pop %rbp    
    pop %rbx    
    pop %rdx    
    pop %rcx
    pop %rax
    sti
    iretq
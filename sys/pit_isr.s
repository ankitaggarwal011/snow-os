.text
.global timer_isr
timer_isr:
    push %rax
    push %rcx
    push %rdx
    push %rbx
    push %rbp
    push %rsi
    push %rdi
    callq pit_intr_handler
    pop %rdi    
    pop %rsi    
    pop %rbp    
    pop %rbx    
    pop %rdx    
    pop %rcx
    pop %rax
    sti
    iretq
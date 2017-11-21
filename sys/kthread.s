.text
.global set_rsp
set_rsp:
    movq %rdi, %rsp
    retq

.global switch_to
switch_to:
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
    push %r12
    push %r13
    push %r14
    push %r15
    push %rdi
    movq (%rdi), %r15
    movq %rsp, (%r15) // first argument is self's task struct in rdi. Save rsp in task_struct->rsp_val, which is at the start
    movq %rsi, (%rdi)
    movq (%rsi), %rsp// second argument (in rsi) is next task's struct.
    // at this point, control has switched to differen't struct's stack. Instructions after this point are exec'd after
    // control comes back to current struct
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
    pop %rax
    // current rsp value should go to callq
    //movq %rsp, %rdi
    //callq set_tss_rsp
    retq
.text
.global set_rsp
set_rsp:
    movq %rdi, %rsp
    retq

.global set_rsp_arg1
set_rsp_arg1:
    movq %rdi, %rsp
    movq %rsi, %rdi
    retq

.global get_rsp_val
get_rsp_val:
    movq %rsp, %rax
    retq

.global put_in_rax
put_in_rax:
    movq %rdi, %rax
    retq

.global setup_forked_kthread_stack
setup_forked_kthread_stack:
    movq $0x0, (%rdi)
    movq %rcx, -8(%rdi)
    movq %rdx, -16(%rdi)
    movq %rbx, -24(%rdi)
    movq %rbp, -32(%rdi)
    movq %rsi, -40(%rdi)
    movq %rdi, -48(%rdi)
    movq %r8, -56(%rdi)
    movq %r9, -64(%rdi)
    movq %r10, -72(%rdi)
    movq %r11, -80(%rdi)
    movq %r12, -88(%rdi)
    movq %r13, -96(%rdi)
    movq %r14, -104(%rdi)
    movq %r15, -112(%rdi)
    movq %rdi, -120(%rdi)
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
    // at this point, control has switched to different struct's stack. Instructions after this point are execd after
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
    retq
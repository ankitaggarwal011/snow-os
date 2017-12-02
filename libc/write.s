.global syscall6
syscall6:
    movq %rdi, %rax
    movq %rsi, %rbx
    pushq %r15 //temp reg
    movq %rcx, %r15
    movq %rdx, %rcx
    movq %r15, %rdx
    movq %r8, %rsi
    movq %r9, %rdi
    pop %r15
    INT $0x80
    retq

.global syscall5
syscall5:
    movq %rdi, %rax
    movq %rsi, %rbx
    pushq %r15 //temp reg
    movq %rcx, %r15
    movq %rdx, %rcx
    movq %r15, %rdx
    movq %r8, %rsi
    pop %r15
    INT $0x80
    retq

.global syscall4
syscall4:
    movq %rdi, %rax
    movq %rsi, %rbx
    pushq %r15 //temp reg
    movq %rcx, %r15
    movq %rdx, %rcx
    movq %r15, %rdx
    pop %r15
    INT $0x80
    retq

.global syscall3
syscall3:
    movq %rdi, %rax
    movq %rsi, %rbx
    movq %rdx, %rcx
    INT $0x80
    retq

.global syscall2
syscall2:
    movq %rdi, %rax
    movq %rsi, %rbx
    INT $0x80
    retq

.global syscall1
syscall1:
    movq %rdi, %rax
    INT $0x80
    retq

.global syscall0
syscall0:
    INT $0x80
    retq


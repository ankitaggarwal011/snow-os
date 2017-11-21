.text
.global _jump_usermode
_jump_usermode:
     movq %rsp, %rax
     pushq $0x23
     pushq %rax
     pushfq
     pushq $0x2B
     pushq %rdi
     iretq
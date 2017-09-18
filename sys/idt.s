.text
.global _x86_load_idt
_x86_load_idt:
  lidt (%rdi)
  movabsq $.done, %r10
  pushq %r10
  lretq
.done:
  movq %rdx, %es
  movq %rdx, %fs
  movq %rdx, %gs
  movq %rdx, %ds
  movq %rdx, %ss
  retq
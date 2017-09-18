.text
.global _x86_load_idt
_x86_load_idt:
  lidt (%rsi)
  retq
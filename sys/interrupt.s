.text
.global enable_interrupts
enable_interrupts:
    sti
    retq

.global disable_interrupts
disable_interrupts:
    cli
    retq

.text
.global page_fault_isr
page_fault_isr:
    callq page_fault_handler
    movq %rsp, %rax
    addq %rax, $0x16
    movq %rax, %rsp
    iretq
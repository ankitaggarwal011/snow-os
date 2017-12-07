.text
.global page_fault_isr
page_fault_isr:
    callq page_fault_handler
    addq $0x8, %rsp
    iretq
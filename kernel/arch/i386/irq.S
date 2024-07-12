.globl _irq0;
.globl _irq1;
.globl _irq2;
.globl _irq3;
.globl _irq4;
.globl _irq5;
.globl _irq6;
.globl _irq7;
.globl _irq8;
.globl _irq9;
.globl _irq10;
.globl _irq11;
.globl _irq12;
.globl _irq13;
.globl _irq14;
.globl _irq15;

.extern irq_handler

irq_common_stub:
    pushal
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds         # Update DS segment register
    mov %ax, %es         # Update ES segment register
    mov %ax, %fs         # Update FS segment register
    mov %ax, %gs         # Update GS segment register


    movl %esp, %eax   // Push us the stack, so that we get an addr to where args start
	push %eax    
    call irq_handler   # Call the fault handler (EIP is preserved)


    pop %eax
    # Restore segment registers
    pop %gs              # Restore GS segment register
    pop %fs              # Restore FS segment register
    pop %es              # Restore ES segment register
    pop %ds              # Restore DS segment register
    popa                 # Restore all general-purpose registers

    # Adjust the stack pointer to remove the pushed error code and ISR number
    addl $8, %esp        # Adjust the stack pointer
    sti
    iret                 # Return from the interrupt (pops CS, EIP, EFLAGS, SS, ESP)

_irq0:
    cli
    push $0
    push $32
    jmp irq_common_stub

_irq1:
    cli
    push $0
    push $33
    jmp irq_common_stub    

_irq2:
    cli
    push $0
    push $34
    jmp irq_common_stub

_irq3:
    cli
    push $0
    push $35
    jmp irq_common_stub    

_irq4:
    cli
    push $0
    push $36
    jmp irq_common_stub

_irq5:
    cli
    push $0
    push $37
    jmp irq_common_stub

_irq6:
    cli
    push $0
    push $38
    jmp irq_common_stub

_irq7:
    cli
    push $0
    push $39
    jmp irq_common_stub

_irq8:
    cli
    push $0
    push $40
    jmp irq_common_stub

_irq9:
    cli
    push $0
    push $41
    jmp irq_common_stub

_irq10:
    cli
    push $0
    push $42
    jmp irq_common_stub

_irq11:
    cli
    push $0
    push $43
    jmp irq_common_stub

_irq12:
    cli
    push $0
    push $44
    jmp irq_common_stub

_irq13:
    cli
    push $0
    push $45
    jmp irq_common_stub

_irq14:
    cli
    push $0
    push $46
    jmp irq_common_stub

_irq15:
    cli
    push $0
    push $47
    jmp irq_common_stub
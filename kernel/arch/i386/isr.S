.globl _isr0
.globl _isr1
.globl _isr2
.globl _isr3
.globl _isr4
.globl _isr5
.globl _isr6
.globl _isr7
.globl _isr8
.globl _isr9
.globl _isr10
.globl _isr11
.globl _isr12
.globl _isr13
.globl _isr14
.globl _isr15
.globl _isr16
.globl _isr17
.globl _isr18
.globl _isr19
.globl _isr20
.globl _isr21
.globl _isr22
.globl _isr23
.globl _isr24
.globl _isr25
.globl _isr26
.globl _isr27
.globl _isr28
.globl _isr29
.globl _isr30
.globl _isr31
 
.extern fault_handler

.global isr_common_stub  # Make the label globally accessible

isr_common_stub:
    pusha                # Save all general-purpose registers
    push %ds             # Save data segment register
    push %es             # Save extra segment register
    push %fs             # Save FS segment register
    push %gs             # Save GS segment register

    # Load the Kernel Data Segment descriptor into AX and update segment registers
    mov $0x10, %ax       # Load immediate value 0x10 into AX
    mov %ax, %ds         # Update DS segment register
    mov %ax, %es         # Update ES segment register
    mov %ax, %fs         # Update FS segment register
    mov %ax, %gs         # Update GS segment register
        
    # Call the fault handler
    call fault_handler   # Call the fault handler (EIP is preserved)
 
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


_isr0:
    cli
    push 0
    push 0
    jmp isr_common_stub

_isr1:
    cli
    push 1
    push 0
    jmp isr_common_stub

_isr2:
    cli
    push 2
    push 0
    jmp isr_common_stub

_isr3:
    cli
    push 3
    push 0
    jmp isr_common_stub

_isr4:
    cli
    push 4
    push 0
    jmp isr_common_stub

_isr5:
    cli
    push 5
    push 0
    jmp isr_common_stub

_isr6:
    cli
    push 6
    push 0
    jmp isr_common_stub

_isr7:
    cli
    push 7
    push 0
    jmp isr_common_stub

_isr8:
    cli
    push 8 
    jmp isr_common_stub

_isr9:
    cli
    push 9
    push 0
    jmp isr_common_stub

_isr10:
    cli
    push 10
    jmp isr_common_stub

_isr11:
    cli
    push 11
    jmp isr_common_stub

_isr12:
    cli
    push 12
    jmp isr_common_stub

_isr13:
    cli
    push 13
    jmp isr_common_stub

_isr14:
    cli
    push 14
    jmp isr_common_stub

_isr15:
    cli
    push 15
    push 0
    jmp isr_common_stub

_isr16:
    cli
    push 16
    push 0
    jmp isr_common_stub

_isr17:
    cli
    push 17
    push 0
    jmp isr_common_stub

_isr18:
    cli
    push 18
    push 0
    jmp isr_common_stub

_isr19:
    cli
    push 19
    push 0
    jmp isr_common_stub

_isr20:
    cli
    push 20
    push 0
    jmp isr_common_stub

_isr21:
    cli
    push 21
    push 0
    jmp isr_common_stub

_isr22:
    cli
    push 22
    push 0
    jmp isr_common_stub

_isr23:
    cli
    push 23
    push 0
    jmp isr_common_stub

_isr24:
    cli
    push 24
    push 0
    jmp isr_common_stub

_isr25:
    cli
    push 25
    push 0
    jmp isr_common_stub

_isr26:
    cli
    push 26
    push 0
    jmp isr_common_stub

_isr27:
    cli
    push 27
    push 0
    jmp isr_common_stub

_isr28:
    cli
    push 28
    push 0
    jmp isr_common_stub

_isr29:
    cli
    push 29
    push 0
    jmp isr_common_stub

_isr30:
    cli
    push 30
    push 0
    jmp isr_common_stub

_isr31:
    cli
    push 31
    push 0
    jmp isr_common_stub

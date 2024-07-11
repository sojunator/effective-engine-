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
    pushal                # Save all general-purpose registers
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

    movl %esp, %eax   // Push us the stack, so that we get an addr to where args start
	push %eax    
    # Call the fault handler
    call fault_handler   # Call the fault handler (EIP is preserved)
 
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

//  0: Divide By Zero Exception
_isr0:
	push $0   // A normal ISR stub that pops a dummy error code to keep a             
	push $0   // uniform stack frame
	jmp isr_common_stub

//  1: Debug Exception
_isr1:
	push $0
	push $1
	jmp isr_common_stub


_isr2:
	push $0
	push $2
	jmp isr_common_stub   

_isr3:
	push $0
	push $3
	jmp isr_common_stub   

_isr4:
	push $0
	push $4
	jmp isr_common_stub   

_isr5:
	push $0
	push $5
	jmp isr_common_stub   

_isr6:
	push $0
	push $6
	jmp isr_common_stub   

_isr7:
	push $0
	push $7
	jmp isr_common_stub   

_isr8:
	push $8      
	jmp isr_common_stub

_isr9:
	push $0
	push $9
	jmp isr_common_stub   

_isr10:
	push $10  
	jmp isr_common_stub 

_isr11:
	push $11  
	jmp isr_common_stub   

_isr12:
	push $12 
	jmp isr_common_stub 

_isr13:
	push $13  
	jmp isr_common_stub 

_isr14:
	push $14  
	jmp isr_common_stub  

_isr15:
	push $0
	push $15
	jmp isr_common_stub   

_isr16:
	push $0
	push $16
	jmp isr_common_stub   

_isr17:
	push $0
	push $17
	jmp isr_common_stub   

_isr18:
	push $0
	push $18
	jmp isr_common_stub  

_isr19:
	push $0
	push $19
	jmp isr_common_stub 

_isr20:
	push $0
	push $20
	jmp isr_common_stub  

_isr21:
	push $0
	push $21
	jmp isr_common_stub  

_isr22:
	push $0
	push $22
	jmp isr_common_stub   

_isr23:
	push $0
	push $23
	jmp isr_common_stub   

_isr24:
	push $0
	push $24
	jmp isr_common_stub  

_isr25:
	push $0
	push $25
	jmp isr_common_stub   

_isr26:
	push $0
	push $26
	jmp isr_common_stub  

_isr27:
	push $0
	push $27
	jmp isr_common_stub   

_isr28:
	push $0
	push $28
	jmp isr_common_stub   

_isr29:
	push $0
	push $29
	jmp isr_common_stub   

_isr30:
	push $0
	push $30
	jmp isr_common_stub   

_isr31:
	push $0
	push $31
	jmp isr_common_stub   

.extern idtptr;
.globl idt_load;

idt_load:
    lidt idtptr
    sti
    ret
		
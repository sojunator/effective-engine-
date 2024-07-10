#include <kernel/interrupts.h>  
 

void install_idt() {
    idtptr.base = &idt;
    idtptr.limit = sizeof(struct idt_entry) * 256 - 1;

    memset(idt, 0, sizeof(struct idt_entry) * 256);

    // run asm
    idt_load();
} 

void idt_set_gate(unsigned num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo = base && 0xFFFF;
    idt[num].base_hi = (base >> 16) && 0xFFFF;
    idt[num].sel = sel;
    idt[num].flags = flags;
}

#include <kernel/interrupts.h>  
#include <kernel/io.h>
#include <stdio.h>
#include <kernel/kb.h>

void fault_handler(struct isr_args * r) {
    /* Is this a fault whose number is from 0 to 31? */

    /* Display the description for the Exception that occurred.
    *  In this tutorial, we will simply halt the system using an
    *  infinite loop */ 
    printf("Interrupt: %d, Error Code: %d\n", r->int_no, r->err_code); 

    for (;;);
 }

 void irq_handler(struct isr_args * r) {

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    if (r->int_no == 32) {
        timer_handler(r);
    } else if (r->int_no == 33) {
        keyboard_handler(r);
    } else {
        printf("IRQ: %d, Error Code: %d\n", r->int_no, r->err_code);    
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
 }

 void timer_phase(int hz) {
    int divisor = 1193180 / hz;
    // Three channels, 
    // 0x40 is channel 0 - IRQ0 
    // 0x41 is channel 1 - system
    // 0x42 is channel 2 - Beep
    //           7654 3210
    // 0x36 = 0b 0011 0110
    // 7-6 bits - CNTR = channel 0
    // 5-4 Bit RW mode = RW
    // 3-1 Is mode = 011 = 3 Square Wave mode
    // BCD = 0 = 16 bit, 1 = 4x BCD

    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF); // Low bit
    outportb(0x40, divisor >> 8); // High bit
 }


/* This will keep track of how many ticks that the system
*  has been running for */
void timer_handler(struct isr_args *r){
    /* Increment our 'tick count' */
    timer_ticks++; 
}


void keyboard_handler(struct isr_args * r) {
    unsigned char scancode;

    scancode = inportb(0x60);
    // 0x80 - 0b10000000
    // topbit means that it was just released
    if (scancode & 0x80) {

    } else {
        // It was pressed
        char input = kbdus[scancode];
        printf("Keyboard hit: %c \n", input);
    }
}

void irq_remap() {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0); 
}

 void install_irq() {
    idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E); 
    idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);

    irq_remap();
 }

void install_idt() {
    idtptr.base = &idt;
    idtptr.limit = sizeof(struct idt_entry) * 256 - 1;

    memset(idt, 0, sizeof(struct idt_entry) * 256);

    idt_set_gate(0,  (unsigned)_isr0,  0x08, 0x8E);
    idt_set_gate(1,  (unsigned)_isr1,  0x08, 0x8E);
    idt_set_gate(2,  (unsigned)_isr2,  0x08, 0x8E);
    idt_set_gate(3,  (unsigned)_isr3,  0x08, 0x8E);
    idt_set_gate(4,  (unsigned)_isr4,  0x08, 0x8E);
    idt_set_gate(5,  (unsigned)_isr5,  0x08, 0x8E);
    idt_set_gate(6,  (unsigned)_isr6,  0x08, 0x8E);
    idt_set_gate(7,  (unsigned)_isr7,  0x08, 0x8E);
    idt_set_gate(8,  (unsigned)_isr8,  0x08, 0x8E);
    idt_set_gate(9,  (unsigned)_isr9,  0x08, 0x8E);
    idt_set_gate(10, (unsigned)_isr10, 0x08, 0x8E);
    idt_set_gate(11, (unsigned)_isr11, 0x08, 0x8E);
    idt_set_gate(12, (unsigned)_isr12, 0x08, 0x8E);
    idt_set_gate(13, (unsigned)_isr13, 0x08, 0x8E);
    idt_set_gate(14, (unsigned)_isr14, 0x08, 0x8E);
    idt_set_gate(15, (unsigned)_isr15, 0x08, 0x8E);
    idt_set_gate(16, (unsigned)_isr16, 0x08, 0x8E);
    idt_set_gate(17, (unsigned)_isr17, 0x08, 0x8E);
    idt_set_gate(18, (unsigned)_isr18, 0x08, 0x8E);
    idt_set_gate(19, (unsigned)_isr19, 0x08, 0x8E);
    idt_set_gate(20, (unsigned)_isr20, 0x08, 0x8E);
    idt_set_gate(21, (unsigned)_isr21, 0x08, 0x8E);
    idt_set_gate(22, (unsigned)_isr22, 0x08, 0x8E);
    idt_set_gate(23, (unsigned)_isr23, 0x08, 0x8E);
    idt_set_gate(24, (unsigned)_isr24, 0x08, 0x8E);
    idt_set_gate(25, (unsigned)_isr25, 0x08, 0x8E);
    idt_set_gate(26, (unsigned)_isr26, 0x08, 0x8E);
    idt_set_gate(27, (unsigned)_isr27, 0x08, 0x8E);
    idt_set_gate(28, (unsigned)_isr28, 0x08, 0x8E);
    idt_set_gate(29, (unsigned)_isr29, 0x08, 0x8E);
    idt_set_gate(30, (unsigned)_isr30, 0x08, 0x8E);
    idt_set_gate(31, (unsigned)_isr31, 0x08, 0x8E);
  
    idt_load();
} 

void idt_set_gate(unsigned num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].always0 = 0;
    idt[num].sel = sel;
    idt[num].flags = flags;
}

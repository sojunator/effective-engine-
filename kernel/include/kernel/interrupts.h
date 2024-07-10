#ifndef INTERRUPTS__H_
#define INTERRUPTS__H_

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;

} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base; 
} __attribute__((packed));



struct isr_args { 
    unsigned int gs, fs, es, ds;      /* pushed the segs last */    
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

struct idt_entry idt[256];
struct idt_ptr idtptr;

void install_idt();
void idt_set_gate(unsigned num, unsigned long base, unsigned short sel, unsigned char flags);
void fault_handler(struct isr_args * r);
// Assembly that loads the idt pointer
extern void idt_load();
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();


#endif
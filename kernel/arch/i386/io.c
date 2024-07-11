#include <kernel/io.h>  

/*
            Master PIC Slave PIC
    Ports:      0x20    0xA0
*/

/*
 Inline asm magic
 asm : output operands : input operands : clobbered registers (aka compiler can't use)
 Output operands is just "store here, =a is EAX which is rv"
 Input operands dN = dx register
*/
unsigned char inportb(unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__("inb %1" : 
                        "=a" (rv) : 
                        "dN" (_port));
    return rv;
}

/*
    More magic
    outb dx, eax 
    where dx = _port
    and eax = _data
*/

void outportb(unsigned short _port, unsigned char _data) {
    __asm__ __volatile__("outb %1, %0" : : "dN" (_port), "a" (_data));
}
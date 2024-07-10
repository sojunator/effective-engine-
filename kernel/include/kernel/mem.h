#ifndef _MEM_H
#define _MEM_H


#include <stdint.h>
#include <stddef.h>

struct gdt_ptr {
    uint16_t size;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));


struct gdt_entry gdt[3];
struct gdt_ptr gdtptr;

void gdt_set_gate(int32_t slot, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity);
void gdt_install(); 

extern void gdt_flush();

#endif
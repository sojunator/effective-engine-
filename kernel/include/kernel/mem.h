#ifndef _MEM_H
#define _MEM_H

#include <kernel/multiboot.h>


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
uint32_t * page_dir = 0x107000;

void gdt_set_gate(int32_t slot, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity);
void gdt_install(); 
void setup_paging();
void get_frames(multiboot_info_t* mbd);

extern void gdt_flush();

#endif
#include <kernel/mem.h>

void gdt_set_gate(int32_t slot, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity) {
    // Lower 16 bits - 0xFFFF
    gdt[slot].base_low = (base & 0xFFFF);
    // move upper 16 bits to lower 16 bits, only keep the lower 8 bits of the upper 16
    gdt[slot].base_middle = (base >> 16) & 0xFF;
    // Only keep the upper 8 bits
    gdt[slot].base_high = (base >> 24) & 0xFF;

    // Limits
    gdt[slot].limit_low = (limit & 0xFFFF);
    gdt[slot].granularity = ((limit >> 16) & 0x0F);

    gdt[slot].granularity |= (granularity & 0xF0);
    gdt[slot].access = access;
}

void gdt_install() {
    gdtptr.size = (sizeof(struct gdt_entry) * 3) - 1;
    gdtptr.base = &gdt;

    // Null segment
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment, base address is 0, limit is 4Gbytes, 4KByte granularity,
    // 32 bit opcodes. Codesegment descriptor. 
    // 0x9A = b1001 1010, which is access
    // 0xCF = b1100 1111, which is granularity
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // 0x92 = b1001 0010
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // assembly that performs LGDT and loads SS, CS, DS
    gdt_flush();
}


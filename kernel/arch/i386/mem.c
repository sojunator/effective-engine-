#include <kernel/mem.h>  
#include <kernel/registers.h>

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

void setup_paging() {
    // Page dir should contain 1024 4 KiB pages, 4GiB
    // 0x1000 = 4KiB
    // b 0000  0001 0000 0000 0000
    // 10 Most significant bits are pos in dir
    // 10 Middle bits table offset 
    // 12 Bits are offset

    /*
        Page DIR
        | 31 - 12 | 11 - 8 | 7    | 6   | 5 | 4   | 3   | 2   | 1   | 0 |
        | Addr    | AVL    | PSS  | AVL | A | PCD | PWT | U/S | R/W | P |
    

        Page table entry 
        | 31 - 12 | 11 - 9 | 8  | 7   | 6 | 5 | 4   | 3   | 2   | 1   | 0 |
        | Addr    | AVL    | G  | PAT | D | A | PCD | PWT | U/S | R/W | P |
    */

   // Current linker script starts with loading kernel to 0x00100000 
   // Which means that 0x0 is free
   page_dir = (uint32_t*)0x0;

    setCR3(page_dir);
    // Setup dir
    // General Idea is given a phyiscal address we know that the 
    // bits 31 - 22 are the index into the page dir aka addr >> 22 (throw away bits)
    // bits 21 - 12 are the index into table dir 
    // The rest of the bits are used by MMU to just calculate the offset

    // Give the dir 16 tables, all 4096 aligned
    for (uint32_t i = 0; i < 16; i++) {
        // Kernel starts at 0x100000, we have to stop before creating choas
        // Setup 0x0000 -   0x400000, which is 4194304
        // meaning we can cram in all the tables before the kernel, yay
        // but when we do, stuff becomes strange as we venture into VGA buffer. 
        // Between 0x0 and 0x100000 is 0xB8000, which will get wrecked.
        

        // Each table takes 1024 entries 
         // 16 tables is equal to 1024 * 16 * 4096 = 0x3E80000, so 65 MiB
        page_dir[i] = (0x1000 * i) + 0x3;
    }

    // Setup table contents - go over all addresses
    for (uint32_t addr = 0; addr < 0x3E80000; addr += 0x1000) {
        uint32_t pdIndex = (addr >> 22);
        uint32_t ptIndex = (addr >> 12) & 0x03FF;

        uint32_t* table =  (uint32_t *)(page_dir[pdIndex] & 0xFFFFF000);

        table[ptIndex] = addr + 0x3;
    }
     
    enablePaging();
}


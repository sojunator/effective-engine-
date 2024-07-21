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

void get_frames(multiboot_info_t* mbd) {
    if (!mbd->flags & MULTIBOOT_INFO_MEM_MAP || !mbd->flags & MULTIBOOT_INFO_MEMORY)
        return;

    printf("mem_lower = 0x%x  mem_upper = 0x%x \n",
            (unsigned int) mbd->mem_lower, (unsigned int) mbd->mem_upper);
 

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
   page_dir = (uint32_t*)0x10A000;

    // Lets clear all bytes for our tables, just in case
    for (uint32_t i = 0; i < 1024 * 16 * 1024; i++) {
        page_dir[i] = 0;
    }

    setCR3(page_dir);
    // Setup dir
    // General Idea is given a phyiscal address we know that the 
    // bits 31 - 22 are the index into the page dir aka addr >> 22 (throw away bits)
    // bits 21 - 12 are the index into table dir 
    // The rest of the bits are used by MMU to just calculate the offset

    // Give the dir 16 tables, all 4096 aligned
    for (uint32_t i = 0; i < 16; i++) {
        // Kernel starts at 0x100000 and ends at 0x107000, we have to stop before creating choas
        // Each table takes 1024 entries 
        // 16 tables is equal to 1024 * 16 * 4096 = 0x3E80000, so 65 MiB
        page_dir[i] = 0x10A000 + (0x1000 * (i + 1)) + 0x3;
    }

    // Setup table contents - go over all addresses
    for (uint32_t addr = 0; addr < 0x3E80000; addr += 0x1000) {
        // Get 10 most significant bits
        uint32_t pdIndex = (addr >> 22);
        // Keep only the 12 middle bits, but remove the most significant two bits
        uint32_t ptIndex = (addr >> 12) & 0x3FF;

        // get the address for the table, but remove the 12 least significant bits
        uint32_t* table =  (uint32_t *)(page_dir[pdIndex] & 0xFFFFF000);
        uint32_t physical_page = addr + 0x3;
        table[ptIndex] = physical_page;
 
 
    }
    dump_table(0, 10);
    enablePaging();
}

void dump_table(uint32_t tableIndex, size_t lastIndex) {
    // If we keep all bits we run into trouble
    uint32_t* page_table =  (uint32_t *)(page_dir[tableIndex] & 0xFFFF000); 
  

    for (uint32_t i = 0; i < lastIndex && i < 1024; i++ ) {    
        printf("Value on pos %d in page table: 0x%x\n", i, page_table[i]);
    }

}

void print_mapping(uint32_t addr) { 
    // Lets go backwards, lowest bits for offset is not relevant
    uint32_t page_dir_index = addr >> 22;

    uint32_t * page_table = (uint32_t *) page_dir[page_dir_index];
    uint32_t page_table_index = (addr >> 12) & 0x3FF;
    printf("Page table addr: 0x%x \n", page_table);
    printf("Value on pos %d in page table: 0x%x\n", page_table_index, page_table[page_table_index]);
}

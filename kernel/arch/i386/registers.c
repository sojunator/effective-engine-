#include <kernel/registers.h>

unsigned int getCR2() {
    unsigned int cr2;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (cr2));
    return cr2;
}


unsigned int getCR3() {
    unsigned int cr2;
    __asm__ __volatile__("mov %%cr3, %0" : "=r" (cr2));
    return cr2;
}


void setCR2(unsigned int cr2) {
    __asm__ __volatile__("mov %0, %%cr2" : : "r" (cr2));
}

void setCR3(uint32_t* page_dir)
{
   __asm__ __volatile__("movl %[dir], %%eax \n\
                       movl %%eax, %%cr3" : : [dir] "r" (page_dir): "%eax");
}

void enablePaging()
{
 __asm__ __volatile__("movl %%cr0, %%eax       \n\
                       orl $0x80000001, %%eax   \n\
                       movl %%eax, %%cr0" : : : "%eax");
}
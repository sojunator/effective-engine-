#include <stddef.h>
#include <stdint.h>
#include <stdio.h> 

#include <kernel/multiboot.h>
/*
	Linker.ld
	Kernel starts at 0x100000 = 1024 * 1024
	All segments in Linker.ld is 4K aligned and 4K in size
	.text
	.rodata
	.data
	.bss is 16KiB = 0x4000, and is 4K aligned, and it comes after 
	
	So stack starts 3 aligned in, = 0x103000
	0x102000 + 0x4000 = 0x106000
	So kernel starts 0x100000 and ends  at 0x107000 - ish
*/


void kernel_main(unsigned long magic, multiboot_info_t *mbi) {
	if(magic != 0x2BADB002) { // Invalid multiboot
		return;
	}

	terminal_initialize();
	printf("Hello, kernel World!\n");

	gdt_install();
	printf("Loading GDT\n"); 
	
	printf("Loading IDT\n");
	install_idt();
	install_irq();
	printf("IDT loading IRQ mappings\n");
	timer_phase(100); 
	printf("Paging setup!\n");   
	setup_paging();   
	printf("Reading memory map\n");   

	get_frames(mbi);
	for(;;);
}

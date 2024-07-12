#include <stdio.h>
 
/*
	Linker.ld
	Kernel starts at 0x100000 = 1024 * 1024
	.text is 4K aligned and depends on the instructions
	.bss is 16KiB = 0x4000, and is 4K aligned, and it comes after 
	.text in Linker.ld

	So stack starts 3 aligned in, = 0x103000
	0x102000 + 0x4000 = 0x107000
	So kernel starts 0x100000 and ends  at 0x107000
*/


void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
	gdt_install();
	printf("Loading GDT\n"); 
	
	printf("Loading IDT\n");
	install_idt();
	install_irq();
	printf("IDT loading IRQ mappings\n");
	timer_phase(100); 
	printf("Lets setup paging!\n");
	setup_paging();

	printf("Pagin enbled \n");
	for(;;);
}

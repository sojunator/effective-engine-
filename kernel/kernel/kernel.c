#include <stdio.h>
 
 
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
	printf("Init done"); 
}

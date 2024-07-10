#include <stdio.h>
 
 
void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
	gdt_install();
	printf("Loading GDT\n"); 
	install_idt();
	printf("Loading IDT\n");

	int test = 0;

	printf("Test interrupts: %d", 123 / test);
}

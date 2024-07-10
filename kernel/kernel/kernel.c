#include <stdio.h>
 
void kernel_main(void) {
	terminal_initialize();

	printf("Hello, kernel World!\n");

	
	gdt_install();
	printf("Loading GDT\n");
	printf("Testing numbers %d is this thing on \n", 40);
	printf("Testing numbers 0x%x is this thing on", 40);
}

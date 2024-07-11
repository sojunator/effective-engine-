# Microkernel

This is a fork of the Meaty-skeleton base template


# Current features
* Installs it's own GDT as we risk overwriting GRUBs
* Installs IDT mapped 0-31, with no handling other than reporting the issues
* Installs IRQ mapped into IDT 32-47
* IRQ0 on IDT 32 tracks ticks at 100hz
* IRQ1 tracks and writes out input from keyboard
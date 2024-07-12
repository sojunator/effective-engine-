#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stddef.h>
#include <stdint.h>

unsigned int getCR2();
unsigned int getCR3();

void setCR2(unsigned int cr2);
void setCR3(uint32_t* page_dir);
void enablePaging();

#endif
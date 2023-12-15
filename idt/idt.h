#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idtr_desc{
	uint16_t limit; //size-1
	uint32_t base; //pointer to descriptor array
} __attribute__((packed));

struct idt_desc{
	uint16_t offset_1;
	uint16_t selector;    // selector thats in our GDT, code selector?
	uint8_t zero;		  // reserved
	uint8_t type_attributes; // types and attributes
	uint16_t offset_2;
} __attribute__((packed));

void idt_init();

#endif

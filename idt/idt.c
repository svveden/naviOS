#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"

struct idt_desc idt_descriptors[NAVIOS_TOTAL_INTERRUPTS]; //table of interrupts
struct idtr_desc idtr_descriptor; //singular interrupt

extern void idt_load(struct idtr_desc *ptr);

void idt_zero(){
	print("\nDivide by zero error, silly\n", 4);
}

void idt_set(int interrupt_number, void *address){
	struct idt_desc* desc = &idt_descriptors[interrupt_number];
	desc->offset_1 = (uint32_t) address & 0x0000FFFF; 		// get lower 16 bits
	desc->selector = KERNEL_CODE_SELECTOR;    				// selector thats in our GDT, code selector?
	desc->zero = 0x00;		  								// reserved
	desc->type_attributes = 0xEE; 							// used interrupt, ring 3 privledge (userspace), interrupt gate_ss, interrupt gate
	desc->offset_2 = (uint32_t) address >> 16;
}

void idt_init(){
	memset(idt_descriptors, 0, sizeof(idt_descriptors)); //create null descriptors
	idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
	idtr_descriptor.base = (uint32_t) idt_descriptors;
	idt_set(0, idt_zero);

	// load interrupt descriptor table
	idt_load(&idtr_descriptor);
}

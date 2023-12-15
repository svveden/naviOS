#ifndef KERNEL_H
#define KERNEL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#include <stddef.h>
#include <stdint.h>
void print(const char* str, char color);
void kernel_main();

#endif

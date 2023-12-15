#include "kernel.h"
#include "idt/idt.h"

uint16_t* video_mem = 0;	// position in memory of video_memory buffer.
int pos = 0;				// cursor position

/** Make_char()
 * The video memory array takes a hex value that is 
 * the ascii value concatenated with a color code. This
 * function calculates that hex value.
 * */
uint16_t make_char(char c, char color){
	return color << 8 | c;
}

/** Display_char()
 * This function displays a character onto the screen by
 * setting the literal video_memory array position to the hex
 * value corresponding to the given character:color.
 * */
int display_char(uint16_t cur_char, int position){
	video_mem[position] = cur_char;
	if(video_mem[position] == cur_char){
		return 1;
	}
	return -1;
}

/** Strlen()
 * This function calculates the length of a string by iterating through
 * it until it hits a null <\0> byte, returning the count.
 * */
size_t strlen(const char* str){
	size_t i = 0;
	while(str[i]){
		i++;
	}
	return i;
}

/** Print()
 * This function takes a pointer to a string, calculates it's length
 * using strlen(), then outputs it to the terminal, tracking position
 * with global variable "pos".
 * */
void print(const char* str, char color){
	size_t len = strlen(str);
	for(int i = 0; i < len; i++){
		if(str[i] == '\n'){
			pos = (((pos/80)+1)*80);
		} else{
			int ret_val = display_char(make_char(str[i], color), pos);
			pos+=1;
			if(ret_val == -1){
				video_mem[25] = make_char('F', 2);
				return;
			}
		}
	}
	return;
}

/** Terminal_Initialize()
 * This function clears the terminal by setting entire video memory array to spaces,
 * erasing BIOS info messages
 * */
void terminal_initialize(){
	video_mem = (uint16_t*)(0xB8000);
	for(int i = 0; i < (VGA_HEIGHT*VGA_WIDTH); i++){
		video_mem[i] = 0x0120;
	}	
	return;
}

/** Logo()
 * This function outputs the "naviOS" logo onto the screen on startup
 * */

void logo(){
	char logo[] = "                   _ _____ _____ \n";
	char logo1[] = "                  (_)  _  /  ___|\n";
	char logo2[] = " _ __   __ ___   ___| | | \\ `--. \n";
	char logo3[] = "| '_ \\ / _` \\ \\ / / | | | |`--. \\\n";
	char logo4[] = "| | | | (_| |\\ V /| \\ \\_/ /\\__/ /\n";
	char logo5[] = "|_| |_|\\__,_| \\_/ |_|\\___/\\____/\n";
	print(logo, 2);
	print(logo1, 3);
	print(logo2, 4);
	print(logo3, 5);
	print(logo4, 6);
	print(logo5, 7);
	return;
}

extern void problem();

void kernel_main(){
	terminal_initialize();
	logo();
	print("\nhello, lain\n",7);
	idt_init();
	// problem();
	return;
}

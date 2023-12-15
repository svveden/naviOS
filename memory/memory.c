#include "memory.h"

/** memset 
 * This function takes void* ptr, and sets all bytes up to <size> to given <c> 
 * */
void *memset(void* ptr, int c, size_t size){
	char *c_ptr = (char *) ptr;
	for(int i = 0; i < size; i++){
		c_ptr[i] = (char) c;
	}
	return ptr;
}


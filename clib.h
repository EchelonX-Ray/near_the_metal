#ifndef _insertion_clib_h
#define _insertion_clib_h

#include <stddef.h>

void print(char* str);
void itoa(register signed int number, register char* buf, register signed int buf_len, register signed int base, register signed int set_width);
void* malloc(size_t size);
void free(void* ptr);
void *memcpy(void* dest, const void* src, size_t n);
unsigned int strlen(register const char* str);
void exit(signed int code);

#endif

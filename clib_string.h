#include "clib.h"

// ASM functions
signed long printf(const char *format, ... );
signed long fprintf(signed int fd, const char *format, ... );
signed long dprintf(signed int fd, const char *format, ... );

// C functions
void print(char* str);
void itoa(register signed int number, register char* buf, register signed int buf_len, register signed int base, register signed int set_width);
signed long _dprintf(signed int fd, const char *format, unsigned long* gen_regs, long double* vec_regs, void* stack_params, unsigned long vector_regs_used);
unsigned int strlen(register const char* str);

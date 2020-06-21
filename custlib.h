#ifndef _insertion_custlib_h
#define _insertion_custlib_h

unsigned int strlen(register const char* str);
void cprintf(char *str);
void citoa(register signed int number, register char* buf, register signed int buf_len, register signed int base, register signed int set_width);

#endif

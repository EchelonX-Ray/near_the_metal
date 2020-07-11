#ifndef _insertion_errno_h
#define _insertion_errno_h

#include <stddef.h>
#include <sys/types.h>

#define errno (*(__errno_location()))

struct __errno_info {
	signed int value;
	pid_t tid;
	struct __errno_info* next;
};

struct __errno_info __errno_base;

void __errno_init_errno();
signed int* __errno_location();

#endif

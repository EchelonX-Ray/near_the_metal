#ifndef _insertion_errno_h
#define _insertion_errno_h

#include "clib.h"

#include <stddef.h>
#include <sys/types.h>

#define errno (*(__errno_location()))

struct __errno_info {
	signed int value;
	pid_t tid;
};

extern struct __errno_info __errno_base;
extern volatile struct __errno_info* __errno_base_threadmem;
extern volatile size_t __errno_base_threadmem_size;
extern volatile unsigned int __errno_base_thread_count;
extern mutex __errno_base_thread_mutex;

void __errno_init_errno();
void __errno_init_thread();
void __errno_destroy_thread();
void __errno_remove_thread_entry(pid_t tid);
void __errno_add_thread_entry(pid_t tid);
signed int* __errno_location();

#endif

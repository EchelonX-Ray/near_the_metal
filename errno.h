#ifndef _insertion_errno_h
#define _insertion_errno_h

#include <stddef.h>
#include <sys/types.h>

#define errno (*(__errno_location()))

struct __errno_info {
	signed int value;
	pid_t tid;
};

struct __errno_info __errno_base;
volatile struct __errno_info* __errno_base_threadmem;
volatile size_t __errno_base_threadmem_size;
volatile unsigned int __errno_base_thread_count;
volatile unsigned int __errno_base_thread_mutex;

void __errno_init_errno();
void __errno_init_thread();
void __errno_destroy_thread();
void __errno_remove_thread_entry(pid_t tid);
void __errno_add_thread_entry(pid_t tid);
signed int* __errno_location();

#endif

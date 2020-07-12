#include "errno.h"
#include "syscalls.h"
#include "clib.h"

void __errno_init_errno() {
	__errno_base.tid = gettid();
	__errno_base.value = 0;
	__errno_base_threadmem = NULL;
	__errno_base_threadmem_size = 0;
	__errno_base_thread_count = 0;
	__errno_base_thread_mutex = 1;
	return;
}

void __errno_init_thread() {
	void* ptr;
	ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == NULL || ptr == (void*)-1) {
		exit(-1);
	}
	__errno_base_threadmem = ptr;
	__errno_base_threadmem_size = PAGE_SIZE;
	__errno_base_thread_count = 0;
	return;
}

void __errno_destroy_thread() {
	munmap((void*)__errno_base_threadmem, __errno_base_threadmem_size);
	return;
}

void __errno_remove_thread_entry(pid_t tid) {
	/*
	if (__errno_base_thread_mutex == 1) {
		__errno_base_thread_mutex = 0;
	}
	*/
	unsigned int j;
	unsigned int i;
	j = 0;
	for (i = 0; i < __errno_base_thread_count; i++) {
		if (__errno_base_threadmem[i].tid == tid) {
			continue;
		} else {
			__errno_base_threadmem[j] = __errno_base_threadmem[i];
			j++;
		}
		i++;
	}
	if (i == j) {
		// This should never happen.  It should not be reachable.
		exit(-1);
	} else {
		__errno_base_thread_count--;
	}
	return;
}

void __errno_add_thread_entry(pid_t tid) {
	if ((__errno_base_thread_count + 1) * sizeof(struct __errno_info) > PAGE_SIZE) {
		exit(-1);
	}
	__errno_base_threadmem[__errno_base_thread_count].tid = tid;
	__errno_base_threadmem[__errno_base_thread_count].value = 0;
	__errno_base_thread_count++;
	return;
}

signed int* __errno_location() {
	pid_t tid = gettid();
	if (__errno_base.tid == tid) {
		return &(__errno_base.value);
	}
	unsigned int i = 0;
	while (i < __errno_base_thread_count) {
		if (__errno_base_threadmem[i].tid == tid) {
			return (signed int*)(&(__errno_base_threadmem[i].value));
		}
		i++;
	}
	// This should never happen.  It should not be reachable.
	return NULL;
}

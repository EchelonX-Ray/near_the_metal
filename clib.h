#ifndef _insertion_clib_h
#define _insertion_clib_h

#include <stddef.h>
//#include <asm/unistd.h>
//#include <asm/unistd.h>
//#include <asm/fcntl.h>
//#include <asm/stat.h>
//#include <linux/mman.h>
#include <sys/types.h>
#include <asm/signal.h>
//#include <stddef.h>

extern size_t PAGE_SIZE;

struct ethread {
	volatile pid_t tid __attribute__((aligned(4)));
	volatile pid_t tid2;
	volatile void* stack_loc;
	volatile size_t stack_size;
	volatile void* retval;
};
typedef struct ethread ethread_t;
struct thread_tmp_args {
	void* args;
	void* (*fn)(void*);
	ethread_t* ethread;
};

struct _mutex {
	volatile unsigned int lock_flag __attribute__((aligned(4))); // Starts count at -1
	volatile unsigned int lock_count;
	volatile pid_t locking_tid;
};
typedef struct _mutex mutex;

void __init_libc();
void __destroy_libc();
signed int new_thread(ethread_t* thread, void* (*fn)(void*), void* arg);
signed int join_thread(ethread_t* thread, void** retval);
void mutex_init(mutex* mut);
void mutex_lock(mutex* mut);
void mutex_unlock(mutex* mut);
signed int sigemptyset(sigset_t *set);
signed int sigaction(signed int signum, const struct sigaction* act, struct sigaction* oldact);
pid_t wait(signed int* wstatus);
pid_t waitpid(pid_t pid, signed int* wstatus, signed int options);
void* calloc(size_t nmemb, size_t size);
void* malloc(size_t size);
void free(void* ptr);
void *memset(void *s, signed int c, size_t n);
void *memcpy(void* dest, const void* src, size_t n);
void exit(signed int code);

#endif

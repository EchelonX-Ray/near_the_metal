#include "clib.h"
#include "syscalls.h"
#include "errno.h"
#include <linux/sched.h>
#include <linux/futex.h>

size_t PAGE_SIZE;

void __init_libc() {
	__errno_init_errno();
	struct stat statbuf;
	if (stat("/dev/mem", &statbuf) < 0) {
		exit(-1);
	}
	PAGE_SIZE = statbuf.st_blksize;
	__errno_init_thread();
	return;
}
void __destroy_libc() {
	/*
	Clean up dynamic memory mapping for thread errno locations.
	
	I'm not sure this is really safe unless I halt or kill all threads first.  This probably should 
	have already happened, but if there are rouge threads, this will probably induce segfaults just 
	prior to program exit as any rouge threads attempt to reference errno.  Such a bug would be very 
	rare since it requires such specific conditions to manifest and would be a nightmare to debug.  
	The kernel will clean up the mapping for us anyway after we exit_group().  We can munmap() 
	safely if(__errno_base_thread_count == 0);.
	*/
	if (__errno_base_thread_count == 0) {
		__errno_destroy_thread();
	}
	return;
}
signed int __new_thread(void* arg) {
	// Retrieve thread information
	struct thread_tmp_args* args;
	args = arg;
	
	// Add errno location for this thread
	__errno_add_thread_entry(args->ethread->tid);
	
	// Call the function
	void* (*fn)(void*);
	void *retptr;
	fn = args->fn;
	retptr = fn(args->args);
	args->ethread->retval = retptr;
	
	// Exit the thread
	_exit(0);
	return 0;
}
signed int new_thread(ethread_t* thread, void* (*fn)(void*), void* arg) {
	pid_t retval;
	struct thread_tmp_args args;
	args.args = arg;
	args.fn = fn;
	args.ethread = thread;
	thread->retval = 0;
	thread->tid = 0;
	thread->stack_size = 8192000 + sizeof(struct thread_tmp_args);
	void* ptr = mmap(0, thread->stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if (ptr == (void*)-1) {
		return -1;
	}
	thread->stack_loc = ptr;
	ptr = (ptr + thread->stack_size) - sizeof(struct thread_tmp_args);
	*((struct thread_tmp_args*)ptr) = args;
	retval = clone(__new_thread, ptr, CLONE_THREAD | CLONE_SIGHAND | CLONE_VM | CLONE_PARENT | CLONE_PARENT_SETTID | CLONE_FILES | CLONE_FS | CLONE_CHILD_CLEARTID, ptr, (void*)(&(thread->tid)), 0, (void*)(&(thread->tid)));
	if (retval == -1) {
		return -1;
	}
	thread->tid2 = retval;
	return 0;
}
signed int join_thread(ethread_t* thread, void** retval) {
	
	// TODO: Implement better error handling
	
	pid_t tid = thread->tid;
	if (tid != 0) {
		futex((void*)(&(thread->tid)), FUTEX_WAIT, tid, NULL, 0, 0);
	}
	if (thread->stack_size != 0) {
		__errno_remove_thread_entry(thread->tid2);
		munmap((void*)thread->stack_loc, thread->stack_size);
		thread->stack_loc = 0;
		thread->stack_size = 0;
		thread->tid = 0;
		if (retval != NULL) {
			*retval = (void*)(thread->retval);
		}
	}
	return 0;
}
void mutex_init(mutex* mut) {
	mut->lock_flag = 1;
	mut->lock_count = 0;
	mut->locking_tid = 0;
}
void mutex_lock(mutex* mut) {
	unsigned int is_non_zero;
	unsigned int value;
	
	__asm__ __volatile__ (	"LOCK incl (%%rcx) \n" \
									: /* Output */ \
									: /* Input */ "c" (&(mut->lock_count)) \
									: /* Clobbers */ "cc"	);
	
	// Atomically decrement "mul->lock_flag" and check Zero-Flag status.
	__asm__ __volatile__ (	"xor %%rdx, %%rdx \n" \
									"LOCK decl (%%rcx) \n" \
									"setnz %%dl \n" \
									: /* Output */ "=d" (is_non_zero) \
									: /* Input */ "c" (&(mut->lock_flag)) \
									: /* Clobbers */ "cc"	);
	
	value = mut->lock_flag;	
	while(is_non_zero == 1) {
		if (value == 0) {
			// Catch the race condition.  
			// 
			// In this case, the lock has been released(And the value 
			// of mut->lock_flag changed) between our atomic instruction 
			// on the value in memory and the retrieval of the value 
			// itself from memory.  Since the value of "mut->lock_flag" is 
			// now "0", we can simply just assume ownership of the lock and 
			// therefore, we don't need to block after all.  So we break out 
			// of the loop.
			break;
		}
		// Match the variable "value" to the expected value 
		//of "mut->lock_flag" in memory
		value++;
		// Since we can't actually take ownership of the lock, 
		// atomically increment the "mul->lock_flag" back up
		__asm__ __volatile__ (	"LOCK incl (%%rcx) \n" \
										: /* Output */ \
										: /* Input */ "c" (&(mut->lock_flag)) \
										: /* Clobbers */ "cc"	);
		
		// Wait for "mul->lock_flag" to change
		// This will not block if "value" does not match "mul->lock_flag"
		// In the event if this, we simply continue the loop to recompute what
		// has changed outside of this thread and retry.
		futex((void*)(&(mut->lock_flag)), FUTEX_WAIT, value, NULL, 0, 0);
		// Atomically decrement the lock flag and check Zero-Flag status.
		__asm__ __volatile__ (	"xor %%rdx, %%rdx \n" \
										"LOCK decl (%%rcx) \n" \
										"setnz %%dl \n" \
										: /* Output */ "=d" (is_non_zero) \
										: /* Input */ "c" (&(mut->lock_flag)) \
										: /* Clobbers */ "cc"	);
		
		value = mut->lock_flag;
	}
	
	mut->locking_tid = gettid();
	return;
}
void mutex_unlock(mutex* mut) {
	unsigned int is_non_zero;
	if (mut->locking_tid != gettid()) {
		return;
	}
	mut->locking_tid = 0;
	__asm__ __volatile__ (	"LOCK incl (%%rcx) \n" \
									: /* Output */ \
									: /* Input */ "c" (&(mut->lock_flag)) \
									: /* Clobbers */ "cc"	);
	
	__asm__ __volatile__ (	"xor %%rdx, %%rdx \n" \
									"LOCK decl (%%rcx) \n" \
									"setnz %%dl \n" \
									: /* Output */ "=d" (is_non_zero) \
									: /* Input */ "c" (&(mut->lock_count)) \
									: /* Clobbers */ "cc"	);
	
	if (is_non_zero == 1) {
		futex((void*)(&(mut->lock_flag)), FUTEX_WAKE, 1, NULL, 0, 0);
	}
	
	return;
}
signed int sigemptyset(sigset_t *set) {
	*set = 0;
	return 0;
}
signed int sigaction(signed int signum, const struct sigaction* act, struct sigaction* oldact) {
	return rt_sigaction(signum, act, oldact, sizeof(sigset_t));
}
pid_t wait(signed int* wstatus) {
	return waitpid(-1, wstatus, 0);
}
pid_t waitpid(pid_t pid, signed int* wstatus, signed int options) {
	return wait4(pid, wstatus, options, NULL);
}
void* calloc(size_t nmemb, size_t size) {
	if (nmemb == 0 || size == 0) {
		return 0;
	}
	size_t max_val = ~0;
	if (max_val / size < nmemb || max_val / nmemb < size) {
		return 0;
	}
	return malloc(nmemb * size);
}
void* malloc(size_t size) {
	if (size == 0) {
		return NULL;
	}
	register void* retval;
	retval = mmap(0, size + sizeof(size_t), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	*((size_t*)retval) = size + sizeof(size_t);
	return retval + sizeof(size_t);
}
void free(void* ptr) {
	if (ptr == NULL) {
		return;
	}
	//signed int retval;
	ptr -= sizeof(size_t);
	/*retval = */munmap(ptr, *((size_t*)ptr));
	return;
}
void *memset(void *s, signed int c, size_t n) {
	while (n > 0) {
		n--;
		((unsigned char*)s)[n] = (unsigned char)(c & 0xFF);
	}
	return s;
}
void *memcpy(void* dest, const void* src, size_t n) {
	while (n > 0) {
		n--;
		((unsigned char*)dest)[n] = ((unsigned char*)src)[n];
	}
	return dest;
}
void exit(signed int code) {
	exit_group(code);
}

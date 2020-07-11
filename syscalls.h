#if !defined(__linux__) || !defined(__x86_64__)
#error "This only works on Linux on x86-64."
#endif

#ifndef _insertion_syscalls_h
#define _insertion_syscalls_h

#include <asm/unistd.h>
#include <linux/mman.h>
#include <sys/types.h>
#include <stddef.h>

#define SYSCALL0_V(callnum) \
	{ __asm__ __volatile__ ("syscall\n" : : "a" (callnum) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL1_V(callnum, arg1) \
	{ __asm__ __volatile__ ("syscall\n" : : "a" (callnum), "D" (arg1) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL2_V(callnum, arg1, arg2) \
	{ __asm__ __volatile__ ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL3_V(callnum, arg1, arg2, arg3) \
	{ __asm__ __volatile__ ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL4_V(callnum, arg1, arg2, arg3, arg4) \
	{ register unsigned long __r10 __asm__("r10") = (unsigned long)arg4; \
	  __asm__ __volatile__ ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3), "r" (__r10) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL5_V(callnum, arg1, arg2, arg3, arg4, arg5) \
	{ register unsigned long __r10 __asm__("r10") = (unsigned long)arg4; \
	  register unsigned long __r8 __asm__("r8") = (unsigned long)arg5; \
	  __asm__ __volatile__ ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3), "r" (__r10), "r" (__r8) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL6_V(callnum, arg1, arg2, arg3, arg4, arg5, arg6) \
	{ register unsigned long __r10 __asm__("r10") = (unsigned long)arg4; \
	  register unsigned long __r8 __asm__("r8") = (unsigned long)arg5; \
	  register unsigned long __r9 __asm__("r9") = (unsigned long)arg6; \
	  __asm__ __volatile__ ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3), "r" (__r10), "r" (__r8), "r" (__r9) : "rcx", "r11", "memory", "cc"); }

#define SYSCALL0_R(retval, callnum) \
	{ __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL1_R(retval, callnum, arg1) \
	{ __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL2_R(retval, callnum, arg1, arg2) \
	{ __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL3_R(retval, callnum, arg1, arg2, arg3) \
	{ __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL4_R(retval, callnum, arg1, arg2, arg3, arg4) \
	{ register unsigned long __r10 __asm__("r10") = (unsigned long)arg4; \
	  __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3), "r" (__r10) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL5_R(retval, callnum, arg1, arg2, arg3, arg4, arg5) \
	{ register unsigned long __r10 __asm__("r10") = (unsigned long)arg4; \
	  register unsigned long __r8 __asm__("r8") = (unsigned long)arg5; \
	  __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3), "r" (__r10), "r" (__r8) : "rcx", "r11", "memory", "cc"); }
#define SYSCALL6_R(retval, callnum, arg1, arg2, arg3, arg4, arg5, arg6) \
	{ register unsigned long __r10 __asm__("r10") = (unsigned long)arg4; \
	  register unsigned long __r8 __asm__("r8") = (unsigned long)arg5; \
	  register unsigned long __r9 __asm__("r9") = (unsigned long)arg6; \
	  __asm__ __volatile__ ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3), "r" (__r10), "r" (__r8), "r" (__r9) : "rcx", "r11", "memory", "cc"); }

signed int write(signed int fd, const void *buf, size_t count);
void _exit(signed int status);
void exit_group(signed int status);
void* mmap(void *addr, size_t length, unsigned int prot, unsigned int flags, signed int fd, off_t offset);
signed int munmap(void *addr, size_t length);
pid_t gettid(void);
pid_t clone(signed int (*fn)(void *), void* stack, unsigned long flags, void* arg, signed int* parent_tid, unsigned long tls, signed int* child_tid);
pid_t fork();
signed int nanosleep(const struct timespec *req, struct timespec *rem);

#endif

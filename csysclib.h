// Require Linux on x86-64
#if !defined(__linux__) || !defined(__x86_64__)
#error "This only works on Linux on x86-64."
#endif

#ifndef _insertion_csysclib_h
#define _insertion_csysclib_h

#include <asm/unistd.h>

#define SYSCALL0_V(callnum) \
	__asm__ volatile ("syscall\n" : : "a" (callnum) : "rcx", "r11", "memory", "cc")
#define SYSCALL1_V(callnum, arg1) \
	__asm__ volatile ("syscall\n" : : "a" (callnum), "D" (arg1) : "rcx", "r11", "memory", "cc")
#define SYSCALL2_V(callnum, arg1, arg2) \
	__asm__ volatile ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2) : "rcx", "r11", "memory", "cc")
#define SYSCALL3_V(callnum, arg1, arg2, arg3) \
	__asm__ volatile ("syscall\n" : : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3) : "rcx", "r11", "memory", "cc")
// #define SYSCALL4_V(callnum, arg1, arg2, arg3, arg4)
// #define SYSCALL5_V(callnum, arg1, arg2, arg3, arg4, arg5)

#define SYSCALL0_R(retval, callnum) \
	__asm__ volatile ("syscall\n" : "=a" (retval) : "a" (callnum) : "rcx", "r11", "memory", "cc")
#define SYSCALL1_R(retval, callnum, arg1) \
	__asm__ volatile ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1) : "rcx", "r11", "memory", "cc")
#define SYSCALL2_R(retval, callnum, arg1, arg2) \
	__asm__ volatile ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2) : "rcx", "r11", "memory", "cc")
#define SYSCALL3_R(retval, callnum, arg1, arg2, arg3) \
	__asm__ volatile ("syscall\n" : "=a" (retval) : "a" (callnum), "D" (arg1), "S" (arg2), "d" (arg3) : "rcx", "r11", "memory", "cc")
// #define SYSCALL4_R(retval, callnum, arg1, arg2, arg3, arg4)
// #define SYSCALL5_R(retval, callnum, arg1, arg2, arg3, arg4, arg5)

int cwrite(int fd, const void *buf, long len);
void cexit_group(int status);

#endif

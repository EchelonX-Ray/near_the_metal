#include "syscalls.h"
#include "errno.h"
#include <asm/unistd.h>
#include <asm/errno.h>

signed int write(signed int fd, const void *buf, size_t count) {
	register signed int retval;
	SYSCALL3_R(retval, __NR_write, fd, buf, count);
	if (retval < 0) {
		errno = -retval;
		retval = -1;
	}
	return retval;
}

void _exit(signed int status) {
	SYSCALL1_V(__NR_exit, status);
	return;
}

void exit_group(signed int status) {
	SYSCALL1_V(__NR_exit_group, status);
	return;
}

void* mmap(void *addr, size_t length, unsigned int prot, unsigned int flags, signed int fd, off_t offset) {
	register void* retval;
	SYSCALL6_R(retval, __NR_mmap, addr, length, prot, flags, fd, offset);
	switch (((signed long)retval)) {
		case -EACCES:
			errno = -((signed long)retval);
			break;
		case -EAGAIN:
			errno = -((signed long)retval);
			break;
		case -EBADF:
			errno = -((signed long)retval);
			break;
		case -EEXIST:
			errno = -((signed long)retval);
			break;
		case -EINVAL:
			errno = -((signed long)retval);
			break;
		case -ENFILE:
			errno = -((signed long)retval);
			break;
		case -ENODEV:
			errno = -((signed long)retval);
			break;
		case -ENOMEM:
			errno = -((signed long)retval);
			break;
		case -EOVERFLOW:
			errno = -((signed long)retval);
			break;
		case -EPERM:
			errno = -((signed long)retval);
			break;
		case -ETXTBSY:
			errno = -((signed long)retval);
			break;
		default:
			return retval;
	}
	return (void*)-1;
}

signed int munmap(void *addr, size_t length) {
	register signed int retval;
	SYSCALL2_R(retval, __NR_munmap, addr, length);
	switch (retval) {
		case -EACCES:
			errno = -retval;
			break;
		case -EAGAIN:
			errno = -retval;
			break;
		case -EBADF:
			errno = -retval;
			break;
		case -EEXIST:
			errno = -retval;
			break;
		case -EINVAL:
			errno = -retval;
			break;
		case -ENFILE:
			errno = -retval;
			break;
		case -ENODEV:
			errno = -retval;
			break;
		case -ENOMEM:
			errno = -retval;
			break;
		case -EOVERFLOW:
			errno = -retval;
			break;
		case -EPERM:
			errno = -retval;
			break;
		case -ETXTBSY:
			errno = -retval;
			break;
		default:
			return retval;
	}
	return -1;
}

pid_t gettid(void) {
	register pid_t retval;
	SYSCALL0_R(retval, __NR_gettid);
	return retval;
}

pid_t clone(signed int (*fn)(void *), void* stack, unsigned long flags, void* arg, signed int* parent_tid, unsigned long tls, signed int* child_tid) {
	register pid_t retval;
	// Note: This syscall's argument order changes depending on architecture.
	// Don't forget to update this with #ifdefs when porting.
	SYSCALL5_R(retval, __NR_clone, flags, stack, parent_tid, child_tid, tls);
	if (retval > 0) {
		// Parent
		return retval;
	} else if (retval == 0) {
		// Child
		retval = fn(arg);
		_exit(retval);
	} else {
		// Error
		errno = -retval;
	}
	return -1;
}

pid_t fork() {
	register pid_t retval;
	SYSCALL0_R(retval, __NR_fork);
	if (retval < 0) {
		errno = -retval;
		return -1;
	}
	return retval;
}

signed int nanosleep(const struct timespec *req, struct timespec *rem) {
	register signed int retval;
	SYSCALL2_R(retval, __NR_nanosleep, req, rem);
	if (retval < 0) {
		errno = -retval;
		return -1;
	}
	return 0;
}
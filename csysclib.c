#include "csysclib.h"
#include <asm/unistd.h>

int cwrite(int fd, const void *buf, long len) {
	register long retval;
	
	if (fd < 0 || !buf || len < 0) {
		return -1;
	}
	
	SYSCALL3_R(retval, __NR_write, fd, buf, len);
	return retval;
}

void cexit_group(int status) {
	SYSCALL1_V(__NR_exit_group, status);
	return;
}

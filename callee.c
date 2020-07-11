#include "syscalls.h"
#include "clib.h"
#include "errno.h"
#include <linux/sched.h>

#define printf print

signed int my_c_thread(void* param) {
	char buff[100];
	printf("Thread2: ");
	itoa(gettid(), buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	struct timespec req;
	struct timespec rem;
	req.tv_sec  =  5;
	req.tv_nsec =  0;
	nanosleep(&req, &rem);
	return 0;
}

signed int main(unsigned int argc, char* argv[], char* envp[]) {
	signed int retval;
	struct timespec req;
	struct timespec rem;
	void *new_stack = 0;
	char buff[100];
	new_stack = malloc(8192000);
	new_stack += 8192000;
	retval = clone(my_c_thread, new_stack, CLONE_THREAD | CLONE_SIGHAND | CLONE_VM, 0, 0, 0, 0);
	//retval = fork();
	if (retval > 0) {
		printf("HereD: ");
		itoa(gettid(), buff, 100, 10, 0);
		printf(buff);
		printf("\n");
		req.tv_sec  = 10;
		req.tv_nsec =  0;
		nanosleep(&req, &rem);
	} else {
		printf("HereF\n");
	}
	return 0;
}

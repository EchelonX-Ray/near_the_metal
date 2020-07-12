#include "syscalls.h"
#include "clib.h"
#include "errno.h"
#include <linux/sched.h>
#include <sys/time.h>

#define printf print

mutex mut;

void* my_c_thread(void* param) {
	//char buff[100];
	mutex_lock(&mut);
	printf("Thread2\n");
	/*
	itoa(gettid(), buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	*/
	/*
	struct timespec req;
	struct timespec rem;
	req.tv_sec  =  5;
	req.tv_nsec =  0;
	*/
	//nanosleep(&req, &rem);
	mutex_unlock(&mut);
	return 0;
}

signed int main(unsigned int argc, char* argv[], char* envp[]) {
	ethread_t thread;
	char buff[100];
	//signed int retval;
	struct timespec req;
	struct timespec rem;
	req.tv_sec  = 5;
	req.tv_nsec =  0;
	mutex_init(&mut);
	mutex_lock(&mut);
	printf("HereA: ");
	itoa(thread.tid, buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	new_thread(&thread, &my_c_thread, 0);
	nanosleep(&req, &rem);
	printf("HereB: ");
	itoa(thread.tid, buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	//void *new_stack = 0;
	//new_stack = malloc(8192000);
	//new_stack += 8192000;
	//retval = clone(my_c_thread, new_stack, CLONE_THREAD | CLONE_SIGHAND | CLONE_VM, 0, 0, 0, 0);
	//retval = fork();
	//if (retval > 0) {
	printf("HereC: ");
	itoa(PAGE_SIZE, buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	/*
	req.tv_sec  =  1;
	req.tv_nsec =  0;
	*/
	//nanosleep(&req, &rem);
	printf("HereD: ");
	itoa(thread.tid, buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	mutex_unlock(&mut);
	join_thread(&thread, 0);
	/*
	req.tv_sec  =  1;
	req.tv_nsec =  0;
	nanosleep(&req, &rem);
	*/
	printf("HereE: ");
	itoa(thread.tid, buff, 100, 10, 0);
	printf(buff);
	printf("\n");
	return 0;
}

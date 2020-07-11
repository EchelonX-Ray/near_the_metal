#include "errno.h"
#include "syscalls.h"

void __errno_init_errno() {
	__errno_base.tid = gettid();
	__errno_base.value = 0;
	__errno_base.next = NULL;
}

signed int* __errno_location() {
	struct __errno_info* __errno;
	__errno = &__errno_base;
	while (__errno != NULL) {
		if (gettid() == __errno->tid) {
			return &(__errno->value);
		} else {
			__errno = __errno->next;
		}
	}
	
	// This should never happen.  It should not be reachable.
	return NULL;
}

void test() {
	errno = 0;
}

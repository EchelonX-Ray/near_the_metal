#include "clib.h"
#include "clib_string.h"
#include "syscalls.h"

signed int main(unsigned int argc, char* argv[], char* envp[]) {
	long double D = 10000000000.3;
	double d = 10000000000.3;
	float f = 10000000000.3;
	
	dprintf(1, "aa%Lfbb\n", D);
	dprintf(1, "cc%fdd\n", d);
	dprintf(1, "ee%fff\n", f);
	dprintf(1, "gg%dhh\n", 10);
	dprintf(1, "ii%sjj\n", "Hello, World!");
	
	fprintf(1, "aa%Lfbb\n", D);
	fprintf(1, "cc%fdd\n", d);
	fprintf(1, "ee%fff\n", f);
	fprintf(1, "gg%dhh\n", 10);
	fprintf(1, "ii%sjj\n", "Hello, World!");
	
	printf("aa%Lfbb\n", D);
	printf("cc%fdd\n", d);
	printf("ee%fff\n", f);
	printf("gg%dhh\n", 10);
	printf("ii%sjj\n", "Hello, World!");
	
	return 0;
}

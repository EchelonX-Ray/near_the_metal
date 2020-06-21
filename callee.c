#include "csysclib.h"
#include "custlib.h"

int my_c_func(int argc, char*argv[], char* envp[]) {
	char* str = "Hello, World!\n";
	char buff[100];
	signed int i;
	
	citoa(argc, buff, 100, 10, 0);
	cprintf("argc: ");
	cprintf(buff);
	cprintf("\n\n");
	
	cprintf("argv: \n");
	i = 0;
	while (i < argc) {
		citoa(i, buff, 100, 10, 0);
		cprintf("\t");
		cprintf(buff);
		cprintf(": ");
		cprintf(argv[i]);
		cprintf("\n");
		i++;
	}
	cprintf("\n");
	
	cprintf("envp: \n");
	i = 0;
	while (envp[i] != 0) {
		citoa(i, buff, 100, 10, 0);
		cprintf("\t");
		cprintf(buff);
		cprintf(": ");
		cprintf(envp[i]);
		cprintf("\n");
		i++;
	}
	cprintf("\n");
	
	return 3;
}

# Porting this project to other CPU Architectures under Linux  
  
No MMU systems are not currently supported, otherwise, except for the exceptions  
listed below, the C code should be portable.  If you find something that is not  
portable, please file a bug or submit a patch.  
  
1) The syscall macros in the syscalls.h header file will need to be rewritten and  
IFDEFed to support the target architecture.  
  
2) The clone syscall changes it's order of arguments depending on CPU architecture.  
This should be rewritten and IFDEFed in the clone syscall C function wrapper in  
syscalls.c  
  
3) For the:  
  
mutex_lock() function in clib.c and 
struct _mutex declaration  
  
the variables:  
  
lock_flag,  
lock_count, and  
value  
  
Should be the same type, and should be exactly 4 bytes in size.  
  
4) The inline assembly inside functions:  
  
mutex_lock() in clib.c and  
mutex_unlock()  
  
should be rewritten and IFDEFed to the target architecture's  
equivalent of atomic instructions  
  
5) The bootstrap _start replacement program will have to rewriten and conditionally  
included by architecture.  This is the program currently called "caller.s" and is the custom  
entry point for the program, replacing "_start" with "my_entry_pt".  
  
6) For architectures under which the stack grows up.  You will have to also rewrite and  
IFDEF the C code which handles Thread creation, argument passing, and destruction in  
"clib.c".  
  
Optional and on my TODO list:  
  
Assembly micro-optimization with C fallback.  

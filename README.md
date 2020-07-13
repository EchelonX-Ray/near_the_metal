# near_the_metal  
Experimenting with ASM and C  
  
Currently, this only works under Linux on x86-64.  
  
The following are also requirements:  
-SSE2 Instruction Support on your CPU  
  
TODO:  
-Clean up custom itoa() implementation  
-Create and run tests to verify the correctness of custom itoa() implementation  
-Assembly micro-optimization with C fallback.  
-Create printf, fprintf, and dprintf replacements  
-Remove SSE2 requirement  
-Update porting guide to include clib_supp.s and print variants  
-Add dynamic expansion for thread errno location storage  
-Implement realloc  
-Make malloc, calloc, and free smart with their memory management  

.text

.globl my_entry_pt

my_entry_pt:
  xor %rbp, %rbp
  
  # Get argc (Located: At the top of the kernel loader initialized stack)
  pop %rdi
  
  # Check that argc is greater than 0
  xor %rdx, %rdx
  cmp %rdi, %rdx
  jc get_argv
  mov $0, %rsi
  mov %rsp, %rax
  jmp get_envp
  
  # Get argv (Located: Just under argc on the stack)
  get_argv:
  mov %rsp, %rsi
  
  mov $8, %rax
  mul %rdi
  add %rsp, %rax
  # Get envp (Located: At the end of argv + 8 zero bytes [argc * 8 + 8], on the stack)
  get_envp:
  add $8, %rax
  mov %rax, %rdx
  
  # Ensure the stack pointer is aligned to a 16 byte boundry
  mov %rsp, %rax
  and $0xF, %rax
  sub %rax, %rsp
  
  # Call the C function to setup errno
  # Save the values we have found for later
  push %rdi
  push %rsi
  push %rdx
  push %rdx # Push rdx again because we need 16 byte stack alignment for ABI conformance
  call __init_libc
  pop %rdx
  pop %rdx
  pop %rsi
  pop %rdi
  
  # Call my C function
  call main # this function has no parameters and no return data
  push %rax
  push %rax
  
  # Clean up
  call __destroy_libc
  
  pop %rax
  pop %rax
  # Make the 'group_exit' system call
  mov %rax, %rdi # set the single parameter, the exit code to 0 for normal exit
  mov $231, %rax # set the syscall to the index of 'group_exit' (231)
  syscall
  
  # Spin if we somehow make it here.  This should not be reachable.
  loop:
  jmp loop
  
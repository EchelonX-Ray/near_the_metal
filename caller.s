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
  jmp get_envp
  
  # Use the envp pointer to demark the cstr termination point
  adjust_terminator:
  add $8, %r9
  mov (%r9), %r11
  jmp ret_pt
  
  # Get and copy to add NULL terminator to argv (Located: Just under argc on the stack)
  get_argv:
  mov $8, %rax
  mul %rdi
  mov %rsp, %r8
  sub %rax, %rsp
  mov %rsp, %rcx
  mov %rcx, %rsi
  
  copy_argv_cstr:
  mov (%r8), %r10
  cmp $0, %r10
  jz get_envp
  mov %r8, %r9
  add $8, %r9
  mov (%r9), %r11
  cmp $0, %r11
  jz adjust_terminator
  ret_pt:
  
  cmp %r11, %r10
  jc write_char_loop_start
  push $0  
  jmp exit_write_char_loop
  
  write_char_loop_start:
    
  mov %r11, %rax
  sub %r10, %rax
  xor %rdx, %rdx
  mov $8, %r9
  div %r9
  cmp $0, %rdx
  jnz not_even_alignment
  push $0
  not_even_alignment:
  sub %rdx, %r11
  mov (%r11), %r9
  xor %rax, %rax
  bitmask_loop:
  shl $8, %rax
  add $0xFF, %rax
  dec %rdx
  cmp $0, %rdx
  jnz bitmask_loop
  and %rax, %r9
  push %r9
  
  write_char_loop:
  
  cmp %r11, %r10
  jnc exit_write_char_loop
  
  sub $8, %r11
  mov (%r11), %r9
  push %r9
  
  jmp write_char_loop
  
  exit_write_char_loop:
  mov %rsp, (%rcx)
  add $8, %rcx
  
  add $8, %r8
  jmp copy_argv_cstr
  
  # Get envp (Located: At the end of argv + 8 zero bytes [argc * 8 + 8], on the stack)
  get_envp:
  mov $8, %rax
  mul %rdi
  add $8, %rax
  add %rax, %rcx
  mov %rcx, %rdx
  call my_c_func # this function has no parameters and no return data
  
  # Make the 'group_exit' system call
  mov %rax, %rdi # set the single parameter, the exit code to 0 for normal exit
  mov $231, %rax # set the syscall to the index of 'group_exit' (231)
  syscall

.text

.globl printf
.globl fprintf
.globl dprintf

printf:
  push %rbp
  mov %rsp, %rbp
  
  # Find the location of any parameters passed to this function on the stack and save it to r10
  mov %rsp, %r10
  add $0x10, %r10
  
  # Push the general-purpose registers used for parameter passing onto the stack in right to left order
  push %r9
  push %r8
  push %rcx
  push %rdx
  push %rsi
  # Save the location of the data to be passed into the C function as the 3rd parameter
  mov %rsp, %rdx
  
  and $0xFF, %rax
  # Save the max count of vector registers used to pass parameters to be passed into the C function as the 6rd parameter
  mov %rax, %r9
  cmp $0, %rax
  jz skip_vector_reg_save0 # No vector regs were used to push data
  
  # Align the stack pointer for the SSE2 Instructions
  mov %rsp, %rax
  and $0xF, %rax
  sub %rax, %rsp
  
  # Push the vector registers used for parameter passing onto the stack in right to left order
  SUB $0x10, %rsp
  MOVAPD %xmm7, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm6, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm5, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm4, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm3, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm2, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm1, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm0, (%rsp)
  # Save the location of the data to be passed into the C function as the 4rd parameter
  mov %rsp, %rcx
  
  skip_vector_reg_save0:
  # Save the location of any parameters passed to this function on the stack and save them to be passed into the C function as the 5rd parameter
  mov %r10, %r8
  
  # Move this functions 1st parameter to the next functions 2nd parameter and use 1 (assumed to be the stdout fd) as the 1st parameter
  mov %rdi, %rsi
  mov $1, %rdi
  
  # Ensure 16 byte stack alignment before calling the C function to conform to the AMD64 ABI spec
  mov %rsp, %rax
  and $0xF, %rax
  sub %rax, %rsp
  
  call _dprintf
  
  mov %rbp, %rsp
  pop %rbp
  ret
  
fprintf:
dprintf:
  push %rbp
  mov %rsp, %rbp
  
  # Find the location of any parameters passed to this function on the stack and save it to r10
  mov %rsp, %r10
  add $0x10, %r10
  
  # Push the general-purpose registers used for parameter passing onto the stack in right to left order
  push %r9
  push %r8
  push %rcx
  push %rdx
  # Save the location of the data to be passed into the C function as the 3rd parameter
  mov %rsp, %rdx
  
  and $0xFF, %rax
  # Save the max count of vector registers used to pass parameters to be passed into the C function as the 6rd parameter
  mov %rax, %r9
  cmp $0, %rax
  jz skip_vector_reg_save1 # No vector regs were used to push data
  
  # Align the stack pointer for the SSE2 Instructions
  mov %rsp, %rax
  and $0xF, %rax
  sub %rax, %rsp
  
  # Push the vector registers used for parameter passing onto the stack in right to left order
  SUB $0x10, %rsp
  MOVAPD %xmm7, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm6, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm5, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm4, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm3, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm2, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm1, (%rsp)
  SUB $0x10, %rsp
  MOVAPD %xmm0, (%rsp)
  # Save the location of the data to be passed into the C function as the 4rd parameter
  mov %rsp, %rcx
  
  skip_vector_reg_save1:
  # Save the location of any parameters passed to this function on the stack and save them to be passed into the C function as the 5rd parameter
  mov %r10, %r8
  
  # The 1st and 2nd parameters are the same between this function and the C function and can simply be left alone in rdi and rsi
  
  # Ensure 16 byte stack alignment before calling the C function to conform to the AMD64 ABI spec
  mov %rsp, %rax
  and $0xF, %rax
  sub %rax, %rsp
  
  call _dprintf
  
  mov %rbp, %rsp
  pop %rbp
  ret

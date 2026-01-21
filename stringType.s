.intel_syntax noprefix

.text

.global string_new
string_new:
  push rbx
  push rbp
  lea rsp, -32[rsp]

  mov rbx, rcx
  mov rbp, [rdx]

  mov ecx, 8[rdx]
  mov 8[rbx], ecx
  lea ecx, 8[ecx]
  and ecx, -8
  mov 12[rbx], ecx

  call malloc
  mov [rbx], rax

  mov rcx, rax
  mov rdx, rbp
  mov r8d, 8[rbx]

  lea rsp, 32[rsp]
  pop rbp
  pop rbx
  jmp memcpy

.global string_append
string_append:
  push rbx
  push rbp

  mov rbx, rcx
  mov rbp, [rdx]
  mov edx, 8[rdx]
  test edx, edx
  jz .string_append_ret

  push rdx
  add edx, 8[rbx]
  mov 8[rbx], edx
  lea edx, 7[edx]
  and edx, -8
  cmp edx, 12[rbx]
  jz .string_append_enoughCap

  lea rsp, -32[rsp]
  mov rcx, [rbx]
  mov 12[rbx], edx
  call realloc
  mov [rbx], rax
  lea rsp, 32[rsp]

.string_append_enoughCap:
  pop rcx
  add rcx, rax
  lea rcx, -1[rcx]
  mov rdx, rbp
  mov r8d, 8[rbx]
  pop rbp
  pop rbx
  jmp memcpy
  
.string_append_ret:
  pop rbp
  pop rbx
  ret

.intel_syntax noprefix

.section rdata

.text

.global string_new
string_new:
  push rbx
  push rbp

  mov rbx, rcx
  mov rbp, [rdx]

  mov ecx, 8[rdx]
  mov 8[rbx], ecx
  lea ecx, 7[rcx]
  and ecx, -8
  mov 12[rbx], ecx
  jz .string_new.return

  lea rsp, -32[rsp]
  call malloc
  lea rsp, 32[rsp]
  
  mov [rbx], rax
  mov rcx, rax
  mov rdx, rbp
  mov r8d, 8[rbx]

  pop rbp
  pop rbx
  jmp memcpy

.string_new.return:
  mov [rbx], rcx
  pop rbp
  pop rbx
  ret

.global string_view
string_view:
  mov rax, [rdx]
  mov [rcx], rax
  mov eax, 8[rdx]
  mov 8[rcx], eax
  ret

.global string_set
string_set:
  xor r8d, r8d
  mov 8[rcx], r8d

.global string_append
string_append:
  mov rax, rdx
  mov edx, 8[rdx]
  test edx, edx
  jz .string_append.return

  push rbx
  push rbp
  mov rbx, rcx
  mov rbp, rax

  push 8[rbx]
  add edx, 8[rbx]
  mov 8[rbx], edx
  lea edx, 7[rdx]
  and edx, -8
  cmp 12[rbx], edx
  jnc .string_append.enoughCap

  lea rsp, -32[rsp]
  mov rcx, [rbx]
  mov 12[rbx], edx
  call realloc
  mov [rbx], rax
  lea rsp, 32[rsp]

.string_append.enoughCap:
  pop rcx
  mov ecx, ecx
  add rcx, [rbx]
  mov rdx, [rbp]
  mov r8d, 8[rbp]
  pop rbp
  pop rbx
  jmp memcpy
  
.string_append.return:
  ret

.intel_syntax noprefix

.text

.global darray_grow
darray_grow: // rdi *darray, esi len, edx type -> void
  mov eax, 12[rdi]
  cmp eax, esi
  jae 1f
  push rbx
  mov rbx, rdi
  0:
  lea ecx, 1[rax]
  shr ecx
  lea eax, 2[rax+rcx]
  and eax, -2
  cmp eax, esi
  jb 0b
  mov 12[rbx], eax
  mov rdi, [rbx]
  imul edx
  mov esi, eax
  call darrayRealloc[rip]
  mov [rbx], rax
  pop rbx
  1:
  ret

.global darray_shrink
darray_shrink: // rdi *darray, esi type -> void
  push rbx
  mov rbx, rdi
  mov rdi, 8[rbx]
  mov edx, edi
  shr rdi, 32
  mov eax, edi
  shr edi
  sub edi, 1
  cmp edx, edi
  ja 1f
  0:
  mov eax, edi
  shr edi
  sub edi, 1
  cmp edx, edi
  jb 0b
  mov 12[rbx], eax
  imul esi, eax
  mov rdi, [rbx]
  call darrayRealloc[rip]
  mov [rbx], rax
  1:
  pop rbx
  ret

.global darray_append
darray_append: // rdi *darray, rsi *element, edx type -> void
  push rbx
  push rsi
  push rdx
  mov rbx, rdi
  mov esi, 8[rbx]
  add esi, 1
  call darray_grow
  mov eax, 8[rbx]
  mov rdi, [rbx]
  pop rcx
  pop rsi
  lea edx, 1[rax]
  mov 8[rbx], edx
  imul eax, ecx
  add rdi, rax
  pop rbx
  jmp  memcpy

.global darray_appendPtr
darray_appendPtr: // rdi *darray, rsi *element -> void
  push rbx
  sub rsp, 16
  mov rbx, rdi
  mov ecx, 8[rdi]
  mov 8[rsp], rsi
  mov [rsp], ecx
  lea esi,1[rcx]
  mov 8[rdi], esi
  mov edx, 8
  call darray_grow
  mov ecx, [rsp]
  mov rsi, 8[rsp]
  mov rdi, [rbx]
  mov [rcx*8+rdi], rsi
  add rsp, 16
  pop rbx
  ret

.global darray_appendMany
darray_appendMany: // rdi *darray, rsi *element, edx amount, ecx type -> void
  push rbx
  push rsi
  push rdx
  mov 4[rsp], ecx
  mov rbx, rdi
  mov esi, 8[rbx]
  add esi, edx
  call darray_grow
  mov eax, 8[rbx]
  mov rdi, [rbx]
  pop rcx
  mov edx, ecx
  lea esi, [rax+rdx]
  mov 8[rbx], esi
  shr rcx, 32
  imul eax, ecx
  imul edx, ecx
  add rdi, rax
  pop rsi
  pop rbx
  jmp  memcpy

.global darray_remove
darray_remove: // rdi *darray, esi index, edx type -> void
  mov ecx, 8[rdi]
  sub ecx, 1
  mov 8[rdi], ecx
  cmp esi, ecx
  jae 0f
  mov rdi, [rdi]
  imul rsi, rdx
  imul rcx, rdx
  sub rcx, rsi
  add rdi, rsi
  lea rsi, [rdx+rdi]
  mov rdx, rcx
  jmp memmove
  0:
  ret

.global darray_removeMany
darray_removeMany: // rdi *darray, esi index, edx amount, ecx type -> void
  mov eax, ecx
  mov ecx, 8[rdi]
  sub ecx, edx
  mov 8[rdi], ecx
  cmp esi, ecx
  jae 0f
  add ecx, edx
  imul rdx, rax
  imul rsi, rax
  imul rcx, rax
  mov rax, [rdi]
  lea rdi, [rax+rsi]
  add rdx, rsi
  sub rcx, rdx
  lea rsi, [rax+rdx]
  mov rdx, rcx
  jmp memmove
  0:
  ret

.global darray_pop
darray_pop: // rdi *darray, esi index, edx type -> void
  mov eax, 8[rdi]
  sub eax, 1
  mov 8[rdi], eax
  cmp esi, eax
  jae 0f
  mov rcx, [rdi]
  imul rax, rdx
  imul rsi, rdx
  lea rdi, [rcx+rsi]
  lea rsi, [rax+rcx]
  jmp memcpy
  0:
  ret

.global darray_popMany
darray_popMany: // rdi *darray, esi index, edx amount, ecx type -> void
  mov eax, 8[rdi]
  sub eax, edx
  mov 8[rdi], eax
  cmp esi, eax
  jae 0f
  push rbx
  imul rax, rcx
  imul rsi, rcx
  imul rdx, rcx
  xor ebx, ebx
  lea rcx, [rdx+rsi]
  sub rcx, rax
  cmovc rcx, rbx
  mov rbx, [rdi]
  add rax, rcx
  lea rdi, [rbx+rsi]
  lea rsi, [rax+rbx]
  sub rdx, rcx
  pop rbx
  jmp memmove
  0:
  ret

.global darray_destroy
darray_destroy: // rdi *darray, esi type -> void
  push rbx
  mov rbx, rdi
  mov ecx, 12[rdi]
  imul ecx, esi
  xor esi, esi
  mov rdi, [rdi]
  call memset
  pxor xmm0, xmm0
  movups [rbx], xmm0
  pop rbx
  mov rdi, rax
  jmp free

.data

.global darrayRealloc
darrayRealloc:
  .quad realloc

.section .note.GNU-stack,"",@progbits

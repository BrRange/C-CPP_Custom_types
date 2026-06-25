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
  lea eax, 1[rax+rcx]
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
  pop rdx
  pop rsi
  lea edx, 1[rax]
  mov 8[rbx], edx
  imul eax, ecx
  add rdi, rax
  pop rbx
  jmp  memcpy

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
  lea esi, [rdx+rax]
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
  cmp ecx, esi
  jz 0f
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
  cmp ecx, esi
  jz 0f
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

.data

.global darrayRealloc
darrayRealloc:
  .quad realloc

.section .note.GNU-stack,"",@progbits


.intel_syntax noprefix

.global darray_grow
darray_grow:
  push rbx
  mov rbx, rcx
  mov ecx, 12[rcx]
  cmp ecx, edx
  jc .darray_grow.notEnough
  mov rax, [rbx]
  pop rbx
  ret
.darray_grow.notEnough:
  xor eax, eax
  test ecx, ecx
  setz al
  lea ecx, [rcx+rax*2]
.darray_grow.growthFactor:
  mov eax, ecx
  shr eax
  lea ecx, 1[rcx+rax]
  and ecx, -2
  cmp ecx, edx
  jc .darray_grow.growthFactor
  lea rsp, -32[rsp]
  mov edx, r8d
  mov 12[rbx], ecx
  imul rdx, rcx
  mov rcx, [rbx]
  call realloc
  mov [rbx], rax
  lea rsp, 32[rsp]
  pop rbx
  ret

.global darray_shrink
darray_shrink:
  mov rax, 8[rcx]
  test eax, eax
  jz darray_destroy
  push rbx
  mov rbx, rcx
  mov rcx, rax
  shl ecx
  shr rax, 32
  cmp eax, ecx
  jnc .darray_shrink.canShrink
  mov rax, [rbx]
  pop rbx
  ret
  .darray_shrink.canShrink:
  shr eax
  lea eax, 1[eax]
  and eax, -2
  cmp eax, ecx
  jnc .darray_shrink.canShrink
  mov 12[rbx], eax
  imul rdx, rax
  mov rcx, [rbx]
  lea rsp, -32[rsp]
  call realloc
  mov [rbx], rax
  lea rsp, 32[rsp]
  pop rbx
  ret

.global darray_append
darray_append:
  push rbx
  push rdx
  push r8
  mov edx, 8[rcx]
  mov eax, r8d
  imul rax, rdx
  lea edx, 1[rdx]
  mov 8[rcx], edx
  mov rbx, rax
  call darray_grow
  pop r8
  pop rdx
  lea rcx, [rax+rbx]
  pop rbx
  jmp memcpy

.global darray_appendPtr
darray_appendPtr:
  push rdx
  mov edx, 8[rcx]
  lea edx, 1[rdx]
  mov 8[rcx], edx
  call darray_grow
  pop rdx
  lea rcx, [rax+rbx]
  mov r8d, 8
  jmp memcpy

.global darray_appendMany
darray_appendMany:
  push rbx
  push rdx
  mov edx, 8[rcx]
  mov eax, r9d
  imul rax, rdx
  lea edx, [rdx+r8]
  imul r8, r9
  push r8
  mov 8[rcx], edx
  mov rbx, rax
  call darray_grow
  pop r8
  pop rdx
  lea rcx, [rax+rbx]
  pop rbx
  jmp memcpy

.global darray_remove
darray_remove:
  mov eax, 8[rcx]
  lea eax, -1[rax]
  mov 8[rcx], eax
  cmp edx, eax
  jnc .darray_remove.noLeftover
  mov rcx, [rcx]
  sub eax, edx
  imul rdx, r8
  lea rcx, [rcx+rdx]
  lea rdx, [rcx+r8]
  imul r8, rax
  jmp memcpy
  .darray_remove.noLeftover:
  ret

.global darray_removeMany
darray_removeMany:
  mov eax, 8[rcx]
  sub eax, r8d
  mov 8[rcx], eax
  cmp edx, eax
  jnc .darray_removeMany.noLeftover
  mov rcx, [rcx]
  imul rdx, r9
  imul r8, r9
  lea rcx, [rcx+rdx]
  lea rdx, [rcx+r8]
  mov r8d, eax
  imul r8, r9
  jmp memcpy
  .darray_removeMany.noLeftover:
  ret

.global darray_pop
darray_pop:
  mov eax, 8[rcx]
  lea eax, -1[rax]
  mov 8[rcx], eax
  cmp edx, eax
  jnc .darray_pop.noLeftover
  mov rcx, [rcx]
  imul rdx, r8
  imul rax, r8
  lea rax, [rcx+rax]
  lea rcx, [rcx+rdx]
  mov rdx, rax
  jmp memcpy
  .darray_pop.noLeftover:
  ret

.global darray_popMany
darray_popMany:
  lea rcx, .unimpl_string[rip]
  call printf
  ud2
  mov eax, 8[rcx]
  sub eax, r8d
  mov 8[rcx], eax
  cmp edx, eax
  jnc .darray_popMany.noLeftover
  mov rcx, [rcx]
  imul rdx, r9
  imul rax, r9
  lea rax, [rcx+rax]
  lea rcx, [rcx+rdx]
  mov rdx, rax
  jmp memcpy
  .darray_popMany.noLeftover:
  ret

.global darray_destroy
darray_destroy:
  pxor xmm0, xmm0
  mov rax, rcx
  mov rcx, [rcx]
  movups [rax], xmm0
  jmp free

.unimpl_string:
  .ascii "Not implemented"
  .byte 10, 0

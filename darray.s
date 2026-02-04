.intel_syntax noprefix

.text

.global darray_grow
darray_grow:
  mov eax, 12[rcx]
  cmp eax, edx
  jc .darray_grow.notEnough
  mov rax, [rcx]
  ret
  .darray_grow.notEnough:
  push rbx
  mov rbx, rcx
  mov ecx, eax
  xor eax, eax
  test ecx, ecx
  setz al
  lea ecx, [rcx+rax*2]
  cmp ecx, edx
  jnc .darray_grow.firstGrowPass
  .darray_grow.growthFactor:
  mov eax, ecx
  shr eax
  lea ecx, 1[rcx+rax]
  and ecx, -2
  cmp ecx, edx
  jc .darray_grow.growthFactor
  .darray_grow.firstGrowPass:
  sub rsp, 32
  mov edx, r8d
  mov r8d, 12[rbx]
  mov 12[rbx], ecx
  imul r8, rdx
  imul rdx, rcx
  mov rcx, [rbx]
  call darrayRealloc[rip]
  mov [rbx], rax
  add rsp, 32
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
  mov r8d, eax
  cmp eax, ecx
  jnc .darray_shrink.canShrink
  mov rax, [rbx]
  pop rbx
  ret
  .darray_shrink.canShrink:
  shr eax
  add eax, 1
  and eax, -2
  cmp eax, ecx
  jnc .darray_shrink.canShrink
  mov 12[rbx], eax
  imul rdx, rax
  mov rcx, [rbx]
  sub rsp, 32
  call darrayRealloc[rip]
  mov [rbx], rax
  add rsp, 32
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
  add edx, 1
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
  push rbx
  push rdx
  mov ebx, 8[rcx]
  lea edx, 1[rbx]
  mov 8[rcx], edx
  mov r8d, 8
  call darray_grow
  pop [rax+rbx*8]
  pop rbx
  ret

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
  mov r8d, r9d
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
  sub eax, 1
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
  sub eax, 1
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
  mov eax, 8[rcx]
  sub eax, r8d
  mov 8[rcx], eax
  cmp edx, eax
  jnc .darray_popMany.noLeftover
  push rbx
  lea ebx, [rdx+r8]
  xor r10d, r10d
  sub ebx, eax
  cmovc ebx, r10d
  mov rcx, [rcx]
  imul rdx, r9
  lea eax, [rax+rbx]
  imul rax, r9
  lea rax, [rcx+rax]
  lea rcx, [rcx+rdx]
  mov rdx, rax
  pop rbx
  imul r8, r9
  jmp memcpy
  .darray_popMany.noLeftover:
  ret

.global darray_recFree
darray_recFree:
  mov edx, 8[rcx]
  test edx, edx
  jz .darray_recFree.noLen
  push rbx
  push rbp
  sub rsp, 32
  mov ebx, edx
  mov rbp, [rcx]
  .darray_recFree.iterate:
  sub ebx, 1
  mov rcx, [rbp+rbx*8]
  call free
  test ebx, ebx
  jnz .darray_recFree.iterate
  add rsp, 32
  pop rbp
  pop rbx
  .darray_recFree.noLen:
  ret

.global darray_destroy
darray_destroy:
  pxor xmm0, xmm0
  mov rax, rcx
  mov rcx, [rcx]
  movups [rax], xmm0
  jmp free

.data

.global darrayRealloc
darrayRealloc:
  .quad realloc

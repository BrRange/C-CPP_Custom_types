.intel_syntax noprefix

.global darray_grow
darray_grow:
  push rbx
  mov rbx, rcx
  mov ecx, 12[rcx]
  cmp ecx, edx
  jc .darray_grow.notEnough
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
  imul edx, ecx
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
  pop rbx
  ret
  .darray_shrink.canShrink:
  shr eax
  lea eax, 1[eax]
  and eax, -2
  cmp eax, ecx
  jnc .darray_shrink.canShrink
  mov 12[rbx], eax
  imul edx, eax
  mov rcx, [rbx]
  lea rsp, -32[rsp]
  call realloc
  mov [rbx], rax
  lea rsp, 32[rsp]
  pop rbx
  ret

.global darray_destroy
darray_destroy:
  pxor xmm0, xmm0
  mov rdx, [rcx]
  movups [rcx], xmm0
  mov rcx, rdx
  jmp free

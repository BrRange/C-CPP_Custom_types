.intel_syntax noprefix

.text

string_grow:
  mov eax, 12[rcx]
  cmp eax, edx
  jc .string_grow.notEnough
  mov rax, [rcx]
  ret
  .string_grow.notEnough:
  push rbx
  mov rbx, rcx
  mov ecx, eax
  xor eax, eax
  test ecx, ecx
  setz al
  lea ecx, [rcx+rax*2]
  cmp ecx, edx
  jnc .string_grow.firstGrowPass
  .string_grow.growthFactor:
  mov eax, ecx
  shr eax
  lea ecx, 1[rcx+rax]
  and ecx, -2
  cmp ecx, edx
  jc .string_grow.growthFactor
  .string_grow.firstGrowPass:
  sub rsp, 32
  mov 12[rbx], ecx
  mov edx, ecx
  mov rcx, [rbx]
  call stringRealloc[rip]
  mov [rbx], rax
  add rsp, 32
  pop rbx
  ret

.global string_new
string_new:
  push rbx
  push rsi
  push rdi
  xor rax, rax
  mov [rcx], rax
  mov rbx, rcx
  mov rsi, [rdx]
  mov edx, 8[rdx]
  mov 8[rcx], rdx
  mov edi, edx
  call string_grow
  mov rcx, rax
  mov rdx, rsi
  mov r8d, edi
  pop rdi
  pop rsi
  sub rsp, 32
  call memcpy
  add rsp, 32
  mov rax, rbx
  pop rbx
  ret

.global string_newView
string_newView:
  mov rax, rcx
  mov [rcx], rdx
  mov 8[rcx], r8d
  ret

.global string_view
string_view:
  mov rax, rcx
  mov rcx, [rdx]
  mov edx, 8[rdx]
  mov [rax], rcx
  mov 8[rax], edx
  ret

.global string_free
string_free:
  mov rax, [rcx]
  pxor xmm0, xmm0
  movups [rcx], xmm0
  mov rcx, rax
  jmp free

.global string_destroy
string_destroy:
  push rcx
  sub rsp, 32
  mov r8d, 8[rcx]
  mov rcx, [rcx]
  xor edx, edx
  call memset
  add rsp, 32
  pop rcx
  pxor xmm0, xmm0
  movups [rcx], xmm0
  mov rcx, rax
  jmp free

.global string_set
string_set:
  push rdx
  mov edx, 8[rdx]
  mov 8[rcx], edx
  call string_grow
  mov rcx, rax
  pop rax
  mov rdx, [rax]
  mov r8d, 8[rax]
  jmp memcpy

.global string_append
string_append:
  push rbx
  push rdx
  mov edx, 8[rdx]
  mov ebx, 8[rcx]
  lea edx, [rbx+rdx]
  mov 8[rcx], edx
  call string_grow
  lea rcx, [rax+rbx]
  pop rax
  pop rbx
  mov r8d, 8[rax]
  mov rdx, [rax]
  jmp memcpy

.global string_findAmount
string_findAmount:
  push rsi
  mov esi, 8[rcx]
  test esi, esi
  jz .string_findAmount.noLen
  push rbx
  xor eax, eax
  mov ebx, eax
  mov rcx, [rcx]
  .string_findAmount.next:
  cmp [rcx], dl
  setz bl
  add eax, ebx
  lea rcx, 1[rcx]
  sub esi, 1
  jnz .string_findAmount.next
  pop rbx
  .string_findAmount.noLen:
  pop rsi
  ret

.global string_findAll
string_findAll: # u32* (const StringView, char, u32 *amount)
  push rbx
  push rbp
  sub rsp, 32
  mov eax, 8[rcx]
  test eax, eax
  jz .string_findAll.noLen
  mov rax, [rcx]
  mov 56[rsp], rax
  mov 64[rsp], edx
  mov 72[rsp], r8
  call string_findAmount
  test eax, eax
  jz .string_findAll.notFound
  mov ebp, eax
  mov rdx, 72[rsp]
  test rdx, rdx
  jz .string_findAll.noStore
  mov [rdx], eax
  .string_findAll.noStore:
  mov ecx, eax
  shl rcx, 2
  call malloc
  mov rcx, 56[rsp]
  mov rbx, rax
  mov edx, 64[rsp]
  xor r8d, r8d
  mov r9d, r8d
  .string_findAll.next:
  cmp [rcx+r8], dl
  jnz .string_findAll.notThisOne
  mov [rax], r8d
  add rax, dword
  add r9d, 1
  .string_findAll.notThisOne:
  add r8d, 1
  cmp r9d, ebp
  jc .string_findAll.next
  mov rax, rbx
  .string_findAll.noLen:
  .string_findAll.notFound:
  add rsp, 32
  pop rbp
  pop rbx
  ret

.global string_findDynamic
string_findDynamic: # void (const StringView, char, void *darray_u32)
  push rbx
  push rbp
  sub rsp, 32
  mov eax, 8[rcx]
  test eax, eax
  jz .string_findDynamic.noLen
  mov rax, [rcx]
  mov 56[rsp], rax
  mov 64[rsp], edx
  mov rbx, r8
  call string_findAmount
  test eax, eax
  jz .string_findDynamic.notFound
  mov ebp, eax
  mov rcx, rbx
  mov edx, 8[rbx]
  add edx, eax
  mov r8d, dword
  call darray_grow
  mov rcx, 56[rsp]
  mov edx, 8[rbx]
  lea rax, [rax+rdx*dword]
  mov edx, 64[rsp]
  add 8[rbx], ebp
  xor r8d, r8d
  mov r9d, r8d
  .string_findDynamic.next:
  cmp [rcx+r8], dl
  jnz .string_findDynamic.notThisOne
  mov [rax], r8d
  add rax, dword
  add r9d, 1
  .string_findDynamic.notThisOne:
  add r8d, 1
  cmp r9d, ebp
  jc .string_findDynamic.next
  .string_findDynamic.noLen:
  .string_findDynamic.notFound:
  add rsp, 32
  pop rbp
  pop rbx
  ret

.data

.global stringRealloc
stringRealloc: # void* (*)(void*, usz)
  .quad realloc

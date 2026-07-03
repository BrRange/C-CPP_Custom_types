.intel_syntax noprefix

.text

.global string_new
string_new: // rdi data, esi len -> rax data, rdx [len, cap]
  push rbx
  push rbp
  sub rsp, 8
  mov rbx, rdi
  mov ebp, esi
  xor edi, edi
  0:
  lea ecx, 1[rdi]
  shr ecx, 1
  lea edi, 2[rdi+rcx]
  and edi, -2
  cmp edi, esi
  jc 0b
  mov [rsp], edi
  mov esi, edi
  xor edi, edi
  call stringRealloc[rip]
  mov rdi, rax
  mov rsi, rbx
  mov edx, ebp
  call memcpy
  mov edx, [rsp]
  shl rdx, 32
  or rdx, rbp
  add rsp, 8
  pop rbp
  pop rbx
  ret

.global string_newView
string_newView: // rdi *txt, esi len -> rax *data, edx len
  mov rax, rdi
  mov edx, esi
  ret

.global string_view
string_view: // rdi *str -> rax *data, edx len
  mov edx, 8[rdi]
  mov rax, [rdi]
  ret

.global string_reserve
string_reserve: // rdi *str, esi cap -> void
  push rbx
  mov rbx, rdi
  mov edx, 12[rdi]
  cmp edx, esi
  jae 1f
  0:
  lea ecx, 1[rdi]
  shr ecx, 1
  lea edi, 2[rdi+rcx]
  and edi, -2
  cmp edx, esi
  jc 0b
  mov 12[rdi], edx
  mov esi, edx
  mov rdi, [rdi]
  call stringRealloc[rip]
  mov [rbx], rax
  1:
  pop rbx
  ret

.global string_append
string_append: // rdi *str, rsi *data, edx len -> void
  push rbx
  sub rsp, 16
  mov rbx, rdi
  mov eax, 8[rdi]
  add 8[rdi], edx
  mov 12[rsp], edx
  mov 8[rsp], eax
  mov [rsp], rsi
  lea esi, [rax+rdx]
  call string_reserve
  mov rdi, [rbx]
  mov edx, 12[rsp]
  mov eax, 8[rsp]
  mov rsi, [rsp]
  add rdi, rax
  add rsp, 16
  pop rbx
  jmp memcpy

.global string_set
string_set: // rdi *str, rsi *data, edx len -> void
  xor eax, eax
  mov 8[rdi], eax
  jmp string_append

.global string_free
string_free: // rdi *str -> void
  mov rsi, rdi
  mov rdi, [rdi]
  pxor xmm0, xmm0
  movdqu [rsi], xmm0
  jmp free

.global string_destroy
string_destroy: // rdi *str -> void
  push rdi
  mov edx, 12[rdi]
  mov rdi, [rdi]
  xor esi, esi
  call memset
  pop rdi
  jmp string_free

.global string_compare
string_compare: // rdi base.data, rsi base.len, rdx target.data, rcx target.len -> eax
  cmp esi, ecx
  jz 0f
  xor eax, eax
  mov ecx, eax
  seta cl
  setb al
  sub eax, ecx
  ret
  0:
  mov rsi, rdx
  mov edx, ecx
  jmp memcmp

.global string_findDynamic
string_findDynamic: // rdi *data, esi len, dl char, rcx *darray -> eax
  push rbx
  push rbp
  movzx edx, dl
  push rdx
  mov rbx, rdi
  mov rbp, rcx
  add rsp, 8
  pop rbp
  pop rbx

.data

.global stringRealloc
stringRealloc:
  .quad realloc

.section .note.GNU-stack,"",@progbits


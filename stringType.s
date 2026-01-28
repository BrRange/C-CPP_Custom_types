.intel_syntax noprefix

.text

string.grow:
  push rbx
  mov rbx, rcx
  mov ecx, 12[rcx]
  cmp ecx, edx
  jc .string.grow.notEnough
  mov rax, [rbx]
  pop rbx
  ret
.string.grow.notEnough:
  xor eax, eax
  test ecx, ecx
  setz al
  lea ecx, [rcx+rax*2]
.string.grow.growthFactor:
  mov eax, ecx
  shr eax
  lea ecx, 1[rcx+rax]
  and ecx, -2
  cmp ecx, edx
  jc .string.grow.growthFactor
  sub rsp, 32
  mov 12[rbx], ecx
  mov edx, ecx
  mov rcx, [rbx]
  call realloc
  mov [rbx], rax
  add rsp, 32
  pop rbx
  ret

.global string_new
string_new: ; String (const StringView)
  ; rcx -> return String*
  ; rdx -> const StringView*
  push rdx
  mov edx, 8[rdx]
  

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

.string_append.return:
  ret

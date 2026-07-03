.intel_syntax noprefix

.data

x8_display.str:
  .asciz "%i.%.*u%s"

.text

.global x8_display
x8_display: // cl fixed, rdx end -> void
  push rdx
  sub rsp, 32
  movzx edx, cl
  mov r9d, edx
  shr edx, 4
  and r9d, 0xf
  xor r8, r8
  0:
  add r8d, 1
  imul r9d, 10
  test r9b, 0xf
  jnz 0b
  shr r9d, 4
  xor eax, eax
  lea rcx, x8_display.str[rip]
  call printf
  add rsp, 40
  ret

.global x8_add
x8_add:
  lea eax, [rcx+rdx]
  ret

.global x8_sub
x8_sub:
  neg dl
  lea eax, [rcx+rdx]
  ret

.global x8_mul
x8_mul:
  mov al, dl
  mul cl
  shr ax, 4
  ret

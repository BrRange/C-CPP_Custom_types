.intel_syntax noprefix

.global binTree_new
binTree_new:
  push rcx
  mov 16[rcx], rdx
  xor edx, edx
  mov 8[rcx], rdx
  mov dl, 8
  lea rcx, [4*rdx]
  sub rsp, 32
  call poolLink_new
  add rsp, 32
  pop rcx
  mov [rcx], rax
  mov rax, rcx
  ret

.global binTree_insert
binTree_insert:
  push rcx
  mov r8, rdx
  mov rdx, 8[rcx]
  sub rsp, 32
  call binTree_insertNode
  add rsp, 32
  pop rcx
  mov 8[rcx], rax
  mov rax, rcx
  ret

.global binTree_free
binTree_free:
  pxor xmm0, xmm0
  mov rdx, rcx
  mov rcx, [rcx]
  movdqu [rdx], xmm0
  jmp poolLink_free

binTree_newNode: // (BinTree*)rcx, (void*)rdx = val -> (Node*)rax
  push rdx
  mov rcx, [rcx]
  sub rsp, 32
  call poolLink_alloc
  add rsp, 32
  xor ecx, ecx
  pxor xmm0, xmm0
  movdqu 8[rax], xmm0
  mov 24[rax], ecx
  pop [rax]
  ret

binTree_insertNode: // (BinTree*)rcx, (Node*)rdx, (void*)r8 = val -> (Node*)rax
  test rdx, rdx
  cmovz rdx, r8
  jz binTree_newNode
  sub rsp, 56
  mov 32[rsp], rcx
  mov 40[rsp], rdx
  mov 48[rsp], r8
  mov rax, 16[rcx] 
  mov rcx, [rdx]
  mov rdx, r8
  call rax
  test eax, eax
  jz binTree_insertNode.endRecursion
  jg binTree_insertNode.right
  mov rcx, 32[rsp]
  mov rax, 40[rsp]
  mov rdx, 8[rax]
  mov r8, 48[rsp]
  call binTree_insertNode
  mov rcx, 40[rsp]
  mov 8[rcx], rax
  jmp binTree_insertNode.endRecursion
  binTree_insertNode.right:
  mov rcx, 32[rsp]
  mov rax, 40[rsp]
  mov rdx, 16[rax]
  mov r8, 48[rsp]
  call binTree_insertNode
  mov rcx, 40[rsp]
  mov 16[rcx], rax
  binTree_insertNode.endRecursion:
  mov rcx, 40[rsp]
  add rsp, 56
  jmp binTree_balance

binTree_balance: // (Node*)rcx -> (Node*)rax
  push rbx
  push rsi
  sub rsp, 40
  mov rbx, rcx
  call binTree_getTilt
  cmp eax, 1
  jg binTree_balance.leftTilt
  cmp eax, -1
  jl binTree_balance.rightTilt
  mov rax, rbx
  binTree_balance.balanced:
  mov rcx, rax
  mov rbx, rax
  call binTree_subHeight
  mov rax, rbx
  add rsp, 40
  pop rsi
  pop rbx
  ret
  binTree_balance.leftTilt:
  mov rsi, 8[rbx]
  mov rcx, rsi
  call binTree_getTilt
  test eax, eax
  jg binTree_balance.leftMirror
  mov rcx, rsi
  call binTree_rotateL
  mov 8[rbx], rax
  binTree_balance.leftMirror:
  mov rcx, rbx
  call binTree_rotateR
  jmp binTree_balance.balanced
  binTree_balance.rightTilt:
  mov rsi, 16[rbx]
  mov rcx, rsi
  call binTree_getTilt
  test eax, eax
  jl binTree_balance.rightMirror
  mov rcx, rsi
  call binTree_rotateR
  mov 16[rbx], rax
  binTree_balance.rightMirror:
  mov rcx, rbx
  call binTree_rotateL
  jmp binTree_balance.balanced

binTree_getTilt: // (Node*)rcx -> (i32)eax
  xor eax, eax
  mov rdx, 8[rcx]
  test rdx, rdx
  jz binTree_getTilt.noLeft
  mov eax, 24[rdx]
  binTree_getTilt.noLeft:
  mov rdx, 16[rcx]
  test rdx, rdx
  jz binTree_getTilt.noRight
  sub eax, 24[rdx]
  binTree_getTilt.noRight:
  ret

binTree_subHeight: // (Node*)rcx -> (i32)eax
  test rcx, rcx
  jz binTree_subHeight.leaf
  push rbx
  sub rsp, 48
  mov rbx, rcx
  mov rcx, 8[rbx]
  call binTree_subHeight
  mov 32[rsp], eax
  mov rcx, 16[rbx]
  call binTree_subHeight
  mov edx, 32[rsp]
  cmp eax, edx
  cmovc eax, edx
  add eax, 1
  mov 24[rbx], eax
  add rsp, 48
  pop rbx
  ret
  binTree_subHeight.leaf:
  xor eax, eax
  ret

binTree_rotateL: // (Node*)rcx -> (Node*)rax
  mov	rax, 16[rcx]
	mov	rdx, 8[rax]
	mov	8[rax], rcx
	mov	16[rcx], rdx
	ret

binTree_rotateR: // (Node*)rcx -> (Node*)rax
  mov	rax, 8[rcx]
	mov	rdx, 16[rax]
	mov	16[rax], rcx
	mov	8[rcx], rdx
	ret

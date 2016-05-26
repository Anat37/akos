	.file	"preass.c"
	.text
	.globl	bitpr
	.type	bitpr, @function
bitpr:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L2
.L7:
	movl	-8(%rbp), %eax
	andl	$7, %eax
	testl	%eax, %eax
	jne	.L3
	cmpl	$0, -8(%rbp)
	je	.L3
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$124, (%rax)
	jmp	.L4
.L3:
	movl	-8(%rbp), %eax
	andl	$3, %eax
	testl	%eax, %eax
	jne	.L4
	cmpl	$0, -8(%rbp)
	je	.L4
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
.L4:
	movl	$31, %eax
	subl	-8(%rbp), %eax
	movl	$1, %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	cltq
	andq	-24(%rbp), %rax
	testq	%rax, %rax
	je	.L5
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$49, (%rax)
	jmp	.L6
.L5:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$48, (%rax)
.L6:
	addl	$1, -8(%rbp)
.L2:
	cmpl	$31, -8(%rbp)
	jle	.L7
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$10, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	movl	$0, -8(%rbp)
	jmp	.L8
.L18:
	movl	$7, %eax
	subl	-8(%rbp), %eax
	sall	$2, %eax
	movl	$15, %edx
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	cltq
	andq	-24(%rbp), %rax
	movq	%rax, %rdx
	movl	$7, %eax
	subl	-8(%rbp), %eax
	sall	$2, %eax
	movl	%eax, %ecx
	sarq	%cl, %rdx
	movq	%rdx, %rax
	movl	%eax, -4(%rbp)
	cmpl	$9, -4(%rbp)
	jg	.L9
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movl	-4(%rbp), %edx
	addl	$48, %edx
	movb	%dl, (%rax)
	jmp	.L10
.L9:
	movl	-4(%rbp), %eax
	subl	$10, %eax
	cmpl	$5, %eax
	ja	.L10
	movl	%eax, %eax
	movq	.L12(,%rax,8), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L12:
	.quad	.L11
	.quad	.L13
	.quad	.L14
	.quad	.L15
	.quad	.L16
	.quad	.L17
	.text
.L11:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$65, (%rax)
	jmp	.L10
.L13:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$66, (%rax)
	jmp	.L10
.L14:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$67, (%rax)
	jmp	.L10
.L15:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$68, (%rax)
	jmp	.L10
.L16:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$69, (%rax)
	jmp	.L10
.L17:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$70, (%rax)
	nop
.L10:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$32, (%rax)
	addl	$1, -8(%rbp)
.L8:
	cmpl	$7, -8(%rbp)
	jle	.L18
	movq	-32(%rbp), %rax
	movb	$10, (%rax)
	addq	$1, -32(%rbp)
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	bitpr, .-bitpr
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits

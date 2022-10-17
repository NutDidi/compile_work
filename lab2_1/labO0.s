	.arch armv7-a
	.fpu vfpv3-d16
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"lab2_1.c"
	.text
	.global	N
	.section	.rodata
	.align	2
	.type	N, %object
	.size	N, 4
N:
	.word	4
	.text
	.align	1
	.global	parameter
	.syntax unified
	.thumb
	.thumb_func
	.type	parameter, %function
parameter:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #4]
	movs	r3, #40
	str	r3, [r7, #12]
	b	.L2
.L7:
	ldr	r1, [r7, #4]
	movw	r3, #21846
	movt	r3, 21845
	smull	r3, r2, r3, r1
	asrs	r3, r1, #31
	subs	r2, r2, r3
	mov	r3, r2
	lsls	r3, r3, #1
	add	r3, r3, r2
	subs	r2, r1, r3
	cmp	r2, #1
	bgt	.L3
	ldr	r3, [r7, #4]
	cmp	r3, #39
	bgt	.L4
.L3:
	ldr	r3, [r7, #4]
	adds	r3, r3, #1
	str	r3, [r7, #4]
	b	.L2
.L4:
	ldr	r3, [r7, #4]
	lsls	r3, r3, #1
	cmp	r3, #70
	bgt	.L5
	ldr	r3, [r7, #4]
	adds	r3, r3, #2
	str	r3, [r7, #4]
	b	.L6
.L5:
	ldr	r3, [r7, #4]
	adds	r3, r3, #3
	str	r3, [r7, #4]
	ldr	r3, [r7, #12]
	subs	r3, r3, #1
	str	r3, [r7, #12]
.L2:
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bge	.L7
.L6:
	ldr	r3, [r7, #4]
	mov	r0, r3
	adds	r7, r7, #20
	mov	sp, r7
	@ sp needed
	ldr	r7, [sp], #4
	bx	lr
	.size	parameter, .-parameter
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d\000"
	.align	2
.LC1:
	.ascii	"result1:%d\012\000"
	.align	2
.LC2:
	.ascii	"X+Y*X-Y=(X+Y)*(X-Y)\000"
	.align	2
.LC3:
	.ascii	"X+Y*X-Y<>(X+Y)*(X-Y)\000"
	.text
	.align	1
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #32
	add	r7, sp, #0
	ldr	r2, .L15
.LPIC4:
	add	r2, pc
	ldr	r3, .L15+4
	ldr	r3, [r2, r3]
	ldr	r3, [r3]
	str	r3, [r7, #28]
	mov	r3, #0
	adds	r3, r7, #4
	mov	r1, r3
	ldr	r3, .L15+8
.LPIC0:
	add	r3, pc
	mov	r0, r3
	bl	__isoc99_scanf(PLT)
	ldr	r3, [r7, #4]
	mov	r0, r3
	bl	parameter(PLT)
	str	r0, [r7, #8]
	movs	r3, #21
	str	r3, [r7, #12]
	movs	r3, #7
	str	r3, [r7, #16]
	movs	r3, #105
	str	r3, [r7, #20]
	bl	rand(PLT)
	mov	r2, r0
	movw	r3, #26215
	movt	r3, 26214
	smull	r1, r3, r3, r2
	asrs	r1, r3, #2
	asrs	r3, r2, #31
	subs	r3, r1, r3
	movs	r1, #10
	mul	r3, r1, r3
	subs	r3, r2, r3
	ldr	r2, [r7, #12]
	add	r2, r2, r3
	ldr	r3, [r7, #16]
	add	r3, r3, r2
	str	r3, [r7, #24]
	ldr	r3, [r7, #24]
	ldr	r2, [r7, #8]
	cmp	r2, r3
	beq	.L10
	ldr	r1, [r7, #8]
	ldr	r3, .L15+12
.LPIC1:
	add	r3, pc
	mov	r0, r3
	bl	printf(PLT)
.L10:
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #12]
	ldr	r1, [r7, #16]
	mul	r3, r1, r3
	cmp	r2, r3
	bne	.L11
	ldr	r3, .L15+16
.LPIC2:
	add	r3, pc
	mov	r0, r3
	bl	printf(PLT)
	b	.L12
.L11:
	ldr	r3, .L15+20
.LPIC3:
	add	r3, pc
	mov	r0, r3
	bl	printf(PLT)
.L12:
	movs	r3, #0
	ldr	r1, .L15+24
.LPIC5:
	add	r1, pc
	ldr	r2, .L15+4
	ldr	r2, [r1, r2]
	ldr	r1, [r2]
	ldr	r2, [r7, #28]
	eors	r1, r2, r1
	mov	r2, #0
	beq	.L14
	bl	__stack_chk_fail(PLT)
.L14:
	mov	r0, r3
	adds	r7, r7, #32
	mov	sp, r7
	@ sp needed
	pop	{r7, pc}
.L16:
	.align	2
.L15:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+4)
	.word	__stack_chk_guard(GOT)
	.word	.LC0-(.LPIC0+4)
	.word	.LC1-(.LPIC1+4)
	.word	.LC2-(.LPIC2+4)
	.word	.LC3-(.LPIC3+4)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC5+4)
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-17ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",%progbits

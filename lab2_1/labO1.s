	.arch armv7-a
	.fpu vfpv3-d16
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 1
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"lab2_1.c"
	.text
	.align	1
	.global	parameter
	.syntax unified
	.thumb
	.thumb_func
	.type	parameter, %function
parameter:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r1, #40
	movw	ip, #21846
	movt	ip, 21845
	b	.L4
.L7:
	adds	r0, r0, #1
.L3:
	cmp	r1, #0
	blt	.L6
.L4:
	smull	r2, r3, ip, r0
	sub	r3, r3, r0, asr #31
	add	r3, r3, r3, lsl #1
	subs	r3, r0, r3
	cmp	r0, #39
	ite	gt
	movgt	r2, #0
	movle	r2, #1
	cmp	r3, #1
	it	gt
	orrgt	r2, r2, #1
	cmp	r2, #0
	bne	.L7
	adds	r0, r0, #3
	subs	r1, r1, #1
	b	.L3
.L6:
	bx	lr
	.size	parameter, .-parameter
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"%d\000"
	.align	2
.LC1:
	.ascii	"result1:%d\012\000"
	.align	2
.LC2:
	.ascii	"X+Y*X-Y<>(X+Y)*(X-Y)\000"
	.text
	.align	1
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	sub	sp, sp, #8
	ldr	r2, .L14
.LPIC4:
	add	r2, pc
	ldr	r3, .L14+4
	ldr	r3, [r2, r3]
	ldr	r3, [r3]
	str	r3, [sp, #4]
	mov	r3, #0
	mov	r1, sp
	ldr	r0, .L14+8
.LPIC0:
	add	r0, pc
	bl	__isoc99_scanf(PLT)
	ldr	r0, [sp]
	bl	parameter(PLT)
	mov	r4, r0
	bl	rand(PLT)
	movw	r3, #26215
	movt	r3, 26214
	smull	r2, r3, r3, r0
	asrs	r2, r0, #31
	rsb	r3, r2, r3, asr #2
	movs	r2, #10
	mls	r3, r2, r3, r0
	adds	r3, r3, #28
	cmp	r3, r4
	bne	.L12
.L9:
	ldr	r1, .L14+12
.LPIC2:
	add	r1, pc
	movs	r0, #1
	bl	__printf_chk(PLT)
	ldr	r2, .L14+16
.LPIC3:
	add	r2, pc
	ldr	r3, .L14+4
	ldr	r3, [r2, r3]
	ldr	r2, [r3]
	ldr	r3, [sp, #4]
	eors	r2, r3, r2
	mov	r3, #0
	bne	.L13
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L12:
	mov	r2, r4
	ldr	r1, .L14+20
.LPIC1:
	add	r1, pc
	movs	r0, #1
	bl	__printf_chk(PLT)
	b	.L9
.L13:
	bl	__stack_chk_fail(PLT)
.L15:
	.align	2
.L14:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+4)
	.word	__stack_chk_guard(GOT)
	.word	.LC0-(.LPIC0+4)
	.word	.LC2-(.LPIC2+4)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC3+4)
	.word	.LC1-(.LPIC1+4)
	.size	main, .-main
	.global	N
	.section	.rodata
	.align	2
	.type	N, %object
	.size	N, 4
N:
	.word	4
	.ident	"GCC: (Ubuntu 11.2.0-17ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",%progbits

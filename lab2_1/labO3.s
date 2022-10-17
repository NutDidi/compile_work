	.arch armv7-a
	.fpu vfpv3-d16
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"lab2_1.c"
	.text
	.align	1
	.p2align 2,,3
	.global	parameter
	.syntax unified
	.thumb
	.thumb_func
	.type	parameter, %function
parameter:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movw	r1, #21846
	movt	r1, 21845
	movs	r2, #40
.L5:
	smull	ip, r3, r1, r0
	sub	r3, r3, r0, asr #31
	add	r3, r3, r3, lsl #1
	subs	r3, r0, r3
	cmp	r3, #2
	it	ne
	cmpne	r0, #39
	ble	.L10
	subs	r2, r2, #1
	adds	r0, r0, #3
	adds	r3, r2, #1
	bne	.L5
	bx	lr
.L10:
	adds	r0, r0, #1
	adds	r3, r2, #1
	bne	.L5
	bx	lr
	.size	parameter, .-parameter
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"%d\000"
	.global	__aeabi_idivmod
	.align	2
.LC1:
	.ascii	"result1:%d\012\000"
	.align	2
.LC2:
	.ascii	"X+Y*X-Y<>(X+Y)*(X-Y)\000"
	.section	.text.startup,"ax",%progbits
	.align	1
	.p2align 2,,3
	.global	main
	.syntax unified
	.thumb
	.thumb_func
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r2, .L25
	ldr	r3, .L25+4
	push	{r4, lr}
.LPIC4:
	add	r2, pc
	ldr	r0, .L25+8
	sub	sp, sp, #8
	ldr	r3, [r2, r3]
	mov	r1, sp
.LPIC0:
	add	r0, pc
	ldr	r3, [r3]
	str	r3, [sp, #4]
	mov	r3, #0
	bl	__isoc99_scanf(PLT)
	ldr	r4, [sp]
	movw	r1, #21846
	movt	r1, 21845
	movs	r2, #40
.L16:
	smull	r0, r3, r1, r4
	sub	r3, r3, r4, asr #31
	add	r3, r3, r3, lsl #1
	subs	r3, r4, r3
	cmp	r3, #2
	it	ne
	cmpne	r4, #39
	ble	.L23
	subs	r2, r2, #1
	adds	r4, r4, #3
	adds	r3, r2, #1
	bne	.L16
	bl	rand(PLT)
.L18:
	ldr	r1, .L25+12
	mov	r2, r4
	movs	r0, #1
.LPIC1:
	add	r1, pc
	bl	__printf_chk(PLT)
	b	.L17
.L23:
	adds	r4, r4, #1
	adds	r0, r2, #1
	bne	.L16
	bl	rand(PLT)
	movs	r1, #10
	bl	__aeabi_idivmod(PLT)
	adds	r1, r1, #28
	cmp	r1, r4
	bne	.L18
.L17:
	ldr	r1, .L25+16
	movs	r0, #1
.LPIC2:
	add	r1, pc
	bl	__printf_chk(PLT)
	ldr	r2, .L25+20
	ldr	r3, .L25+4
.LPIC3:
	add	r2, pc
	ldr	r3, [r2, r3]
	ldr	r2, [r3]
	ldr	r3, [sp, #4]
	eors	r2, r3, r2
	mov	r3, #0
	bne	.L24
	movs	r0, #0
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L24:
	bl	__stack_chk_fail(PLT)
.L26:
	.align	2
.L25:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC4+4)
	.word	__stack_chk_guard(GOT)
	.word	.LC0-(.LPIC0+4)
	.word	.LC1-(.LPIC1+4)
	.word	.LC2-(.LPIC2+4)
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC3+4)
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

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
	push	{r7}
	sub	sp, sp, #20
	add	r7, sp, #0
	str	r0, [r7, #4]
	mov	r3, #40
	str	r3, [r7, #12]
	b	.L2
.L7:
	ldr	r1, [r7, #4]    @r1=n
	movw	r3, #21846
	movt	r3, 21845
	smull	r3, r2, r1, r3
	asrs	r3, r1, #31
	sub	r2, r2, r3
	lsl	r3, r2, #1
	add	r3, r3, r2
	sub	r2, r1, r3      @r2=n%3
	cmp	r2, #1          @n%3<=1?
	bgt	.L3
	cmp	r1, #34         @n<=34    
	bgt	.L4
.L3:
	add	r1, r1, #1      @n=n+1
	str	r1, [r7, #4]
	b	.L2
.L4:
	ldr	r1, [r7, #4]    
	add	r2, r1, r1      @r2=2*n
	cmp	r2, #70         @r2<=70? 
	bgt	.L5
	add	r1, r1, #2      @n=n+2
	str	r1, [r7, #4]
	b	.L6
.L5:
	add	r1, r1, #3      @n=n+3 
	str	r1, [r7, #4]     
	ldr	r2, [r7, #12]   
	subs	r2, r2, #1      @r2=i
	str	r2, [r7, #12]
.L2:
	ldr	r2, [r7, #12]  @r2=i
	cmp	r2, #0         @i>=0?
	bge	.L7
.L6:
	ldr	r0, [r7, #4]
	adds	r7, r7, #20
	mov	sp, r7
	ldr	r7, [sp], #4
	bx	lr
	.size	parameter, .-parameter
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d\000"
.LC1:
	.ascii	"result1:%d\012\000"
.LC2:
	.ascii	"X+Y*X-Y=(X+Y)*(X-Y)\000"
.LC3:
	.ascii	"X+Y*X-Y<>(X+Y)*(X-Y)\000"
	.text
	.global	main
	.type	main, %function
main:
	push	{r7, lr} @lr是返回地址 
	sub	sp, sp, #32 @栈指针上移
	add	r7, sp, #0 @将栈指针的地址赋给r7
	ldr	r2, .L15  @将L15的地址赋给r2
.LPIC4:
	add	r2, pc @pc指令地址
	ldr	r3, .L15+4 @.L15+4加载到r3
	ldr	r3, [r2, r3] @R2+R3加载到r3
	ldr	r3, [r3]
	str	r3, [r7, #28]
	adds	r1, r7, #4
	ldr	r3, .L15+8
.LPIC0:
	add	r3, pc
	mov	r0, r3
	bl	__isoc99_scanf(PLT) @调用scanf()函数
	ldr	r0, [r7, #4] @r7+4是n
	bl	parameter(PLT) @调用parameter
	str	r0, [r7, #8] @ result1是r7+8
	movs	r3, #21   @ a[0]=X+Y;
	str	r3, [r7, #12] @ r7+12是a[0]
	movs	r3, #7   @ a[1]=X-Y;
	str	r3, [r7, #16] @ r7+16是a[1]
	movs	r3, #105 @ a[2]=X+Y*X-Y;
	str	r3, [r7, #20] @ r7+20是a[2]
	bl	rand(PLT)  @调用rand()
	mov	r2, r0  @调用rand后的返回值
	movw	r3, #26215 @把 16 位立即数放到寄存器的底16位，高16位清0
	movt	r3, 26214  @把 16 位立即数放到寄存器的高16位，低16位不影响 
	smull	r1, r3, r3, r2 @计算两个32位有符号数据的乘积，并将得到的64位的结果放到两个32位的寄存器中 Signed (R1,R3) = R3 x R2
	asrs	r1, r3, #2 @ 算数右移
	asrs	r3, r2, #31
	subs	r3, r1, r3
	movs	r1, #10
	mul	r3, r1, r3
	subs	r3, r2, r3 @此时r3为rand()%10
	ldr	r2, [r7, #12] @得到a[0]
	add	r2, r2, r3
	ldr	r3, [r7, #16] @得到a[1]
	add	r3, r3, r2 
	str	r3, [r7, #24]  @ r7+24是a[3]
	ldr	r2, [r7, #8] @ result1放入r2
	cmp	r2, r3 @比较a[3]和result1
	beq	.L10 @相等则跳转
	ldr	r1, [r7, #8] @ result1放入r1
	ldr	r3, .L15+12 
.LPIC1:
	add	r3, pc @定位到打印出"result1:%d\n"的地方
	mov	r0, r3 
	bl	printf(PLT)
.L10:
	ldr	r2, [r7, #20] @得到a[2] 放入r2
	ldr	r3, [r7, #12] @得到a[0] 放入r3
	ldr	r1, [r7, #16] @得到a[1] 放入r1
	mul	r3, r1, r3 @r3=a[0] * a[1]
	cmp	r2, r3 @比较a[2]和a[0] * a[1]
	bne	.L11 @不等则跳转
	ldr	r3, .L15+16 
.LPIC2:
	add	r3, pc @定位到"X+Y*X-Y=(X+Y)*(X-Y)\000"
	mov	r0, r3
	bl	printf(PLT)
	b	.L12
.L11:
	ldr	r3, .L15+20 
.LPIC3:
	add	r3, pc @定位到printf("X+Y*X-Y<>(X+Y)*(X-Y)");
	mov	r0, r3
	bl	printf(PLT)
.L12:
	movs	r3, #0 
	ldr	r1, .L15+24
.LPIC5:
	add	r1, pc @定位到_GLOBAL_OFFSET_TABLE_
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

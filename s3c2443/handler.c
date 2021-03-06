#include <include.h>
#include "../src/include/kernel.h"

/* change_PCB */
void change_PCB(void)
{
	curPCB = highPCB; /* PCB를 교체한다 */
}

/* change_Prio */
void change_Prio(void)
{
	curPrio = nextPrio; /* 최우선순위를 현재순위에 넣는다 */
}

/* Debuging (stack check) */
void debug_reg(unsigned int *sp, unsigned int cpsr, unsigned int spsr)
{
	printf(" STACK POINTER: %p\n", sp); /* 스택포인터 출력 */
	printf("┏━[ HIGH PRIO ]━┓\n");
	printf("┃ SPSR: %08x┃\n", spsr); /* SPSR 출력 */
	printf("┃ CPSR: %08x┃\n", cpsr); /* CPSR 출력 */
	printf("┃ R0:\t%08x┃\n", *(sp++)); /* R0 출력 */
	printf("┃ R1:\t%08x┃\n", *(sp++)); /* R1 출력 */
	printf("┃ R2:\t%08x┃\n", *(sp++)); /* R2 출력 */
	printf("┃ R3:\t%08x┃\n", *(sp++)); /* R3 출력 */
	printf("┃ R4:\t%08x┃\n", *(sp++)); /* R4 출력 */
	printf("┃ R5:\t%08x┃\n", *(sp++)); /* R5 출력 */
	printf("┃ R6:\t%08x┃\n", *(sp++)); /* R6 출력 */
	printf("┃ R7:\t%08x┃\n", *(sp++)); /* R7 출력 */
	printf("┃ R8:\t%08x┃\n", *(sp++)); /* R8 출력 */
	printf("┃ R9:\t%08x┃\n", *(sp++)); /* R9 출력 */
	printf("┃ R10:  %08x┃\n", *(sp++)); /* R10 출력 */
	printf("┃ R11:  %08x┃\n", *(sp++)); /* R11 출력 */
	printf("┃ R12:  %08x┃\n", *(sp++)); /* R12 출력 */
	printf("┃ LR:\t%08x┃\n", *(sp++)); /* link resister 출력 */
	printf("┃ PC:\t%08x┃\n", *(sp++)); /* program counter 출력 */
	printf(" ================\n");
}

/* PCB Stack initialize */
STACK *PCBStackInit(void (*proc)(void *pd), STACK *stktop, void *data)
{
	STACK *stack;
	stack = stktop;

	*(--stack) = (unsigned long)proc; /* PC */
	*(--stack) = (unsigned long)0; /* LR */
	*(--stack) = (unsigned long)0; /* r12 */
	*(--stack) = (unsigned long)0; /* r11 */
	*(--stack) = (unsigned long)0; /* r10 */
	*(--stack) = (unsigned long)0; /* r9 */
	*(--stack) = (unsigned long)0; /* r8 */
	*(--stack) = (unsigned long)0; /* r7 */
	*(--stack) = (unsigned long)0; /* r6 */
	*(--stack) = (unsigned long)0; /* r5 */
	*(--stack) = (unsigned long)0; /* r4 */
	*(--stack) = (unsigned long)0; /* r3 */
	*(--stack) = (unsigned long)0; /* r2 */
	*(--stack) = (unsigned long)0; /* r1 */
	*(--stack) = (unsigned long)0; /* r0 */
	*(--stack) = (unsigned long)0x00000013L; /* CPSR : irq 활성화 & SVC 모드 */

	return (stack); /* 스택의 top을 반환한다 */
}

/* timer initialize */
void os_timer_init(void)
{
	TCFG0 = (TCFG0 & ~(0xFF << 8)) | (255 << 8); // Prescaler 1 Setting...
	TCFG1 = (TCFG1 & ~(0xF << 12)) | (TCFG1_MUX3)| (TCFG1_MUX2) | (TCFG1_MUX1) | (TCFG1_MUX0); // Divider 1/16 Setting...
	INTMSK = 0xFFFFEFFF; /* 0을 서비스 요청 1은 서비스 불가(SRCPND) */
	INTMOD = 0; /* 0은 IRQ 1은 FIQ. ARM은 IRQ만지원하므로 전체 0 */
	TCNTB2 =PCLK/256/16;
	TCMPB2 = 0;
	TCON  = (TCON_TM2_UPDATE_MANUAL);
}

/* timer starting */
void os_timer_start(void)
{
	TCON = (TCON & 0xFFFF0FFF) | (TCON_TM2_AUTO_RELOAD | TCON_TM2_START);
}

/* irq handler */
void irqHandler(void)
{
	if(INTPND == 0x1000){
		SRCPND=0x1000;
		INTPND=0x1000;
	}
}

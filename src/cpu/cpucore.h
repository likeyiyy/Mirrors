/*************************************************************************
	> File Name: cpucore.h
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Oct 2014 09:44:22 PM CST
 ************************************************************************/

#ifndef _CPUCORE_H
#define _CPUCORE_H
/* Total 32       */
/* 1. 算数指令  6 */
#define ADD		0
#define ADDI	1
#define SUB		2
#define MUL		3
#define DIV		4
#define MOD		5
/* 2. 逻辑指令  7 */
#define AND		6
#define OR		7
#define NOT		8
#define XOR	    9
#define ANDI	10
#define ORI		11
#define XORI	12
/* 3. 移位指令  4 */
#define SLL		13
#define SLR		14
#define SAL		15
#define SAR		16
/* 4. 转移指令  11 */
#define LESS	17
#define GREAT	18
#define LESSE	19
#define GREATE	20

#define LESSU	21
#define GREATU	22
#define LESSEU	23
#define GREATEU	24

#define EQUAL	25
#define UEQUAL	26
#define JMP		27
/* 5. 存储指令  4 */
#define MOV		28
#define LWORD	29
#define SWORD	30
#define LUI		31


#define RD(n) cpu->reg[((n >> 21) & 0x1f)]
#define RS(n) cpu->reg[((n >> 16) & 0x1f)]
#define RT(n) cpu->reg[((n >> 11) & 0x1f)]
#define PWD_DIVZERO (1)
#define PWD_MODZERO (1 << 1)
#define PWD_INVINST (1 << 2)
typedef struct _cpu
{
    uint32_t  CPUID;
    uint32_t  PC;
    uint32_t  PWD;
    uint32_t  reg[32];
    memory_t  * memory;
}cpu_t;

cpu_t * init_cpu(uint32_t cpuid,uint32_t pc,memory_t * memory);
void * cpu_main_loop(void * arg);
#endif

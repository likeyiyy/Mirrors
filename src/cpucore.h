/*************************************************************************
	> File Name: cpucore.h
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Oct 2014 09:44:22 PM CST
 ************************************************************************/

#ifndef _CPUCORE_H
#define _CPUCORE_H
/* 1. 算数指令  6 */
#define ADD		0
#define ADDI	1
#define SUB		2
#define MUL		3
#define DIV		4
#define MOD		5
/* 2. 逻辑指令  7 */
#define AND		10
#define OR		11
#define NOT		12
#define XOR		13
#define ANDI	14
#define ORI		15
#define XORI	16
/* 3. 移位指令  4 */
#define SLL		17
#define SLR		18
#define SAL		19
#define SAR		20
/* 4. 转移指令  11 */
#define LESS	21
#define GREAT	22
#define GREATE	23
#define LESSE	24
#define LESSU	25
#define GREATU	26
#define GREATEU	27
#define LESSEU	28
#define EQUAL	29
#define UEQUAL	30
#define JMP		31
/* 5. 存储指令  5 */
#define MOV		32
#define LWORD	33
#define SWORD	34
#define LUI		35
#define HALT	42


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

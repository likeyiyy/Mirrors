/*************************************************************************
	> File Name: cpucore.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Tue 06 May 2014 05:57:59 PM CST
 ************************************************************************/
#ifndef CPUCORE_H
#define CPUCORE_H

#define CURRENT_STATE 0
#define PAGE_FALUT_A  2
#define PDTR          3


#define PAGE_PRESENT(page)  (page & 0x1) 
#define PAGE_ALIGN          2
#define PROTECTED_MODE          0x1UL
#define PAGE_DIRECT_MASK        0XFFC00000
#define PAGE_DIRECT_SHIFT       22
#define PAGE_DIRECT(va) ((va & PAGE_DIRECT_MASK) >> PAGE_DIRECT_SHIFT)

#define PAGE_TABLE_MASK         0X003ff000
#define PAGE_TABLE_SHIFT        12
#define PAGE_TABLE(va)  ((va & PAGE_TABLE_MASK ) >> PAGE_TABLE_SHIFT)

#define PAGE_P_MASK             0x00000fff
#define PAGE_P_SHIFT(va)  (va & PAGE_P_MASK)


#define I_MASK                  0x00000100
#define I_HAPPEN(cpu) (cpu->spr[CURRENT_STATE] & I_MASK)
#define I_CLEAR(cpu)  (cpu->spr[CURRENT_STATE] &= ~I_MASK)
#define I_SET(cpu)    (cpu->spr[CURRENT_STATE] |= I_MASK)

typedef enum inter_verctor
{
    I_DIV_ZERO = 0,
    I_PAGE_FAULT = 3,
};


typedef struct _cpucore
{
    uint32_t address_register;
    uint32_t data_register;
    uint32_t interrupt_vector;
    uint32_t gpr[32];
    uint32_t spr[4];
    memory_t * memory;
}cpucore_t;



cpucore_t * get_cpu();

void * cpu_main_loop(void * arg);

#endif

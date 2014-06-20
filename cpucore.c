/*************************************************************************
	> File Name: cpucore.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 05:14:06 PM CST
 ************************************************************************/

#include "includes.h"

cpucore_t * init_cpu()
{
    cpucore_t * tcpu = malloc(sizeof(cpucore_t));
    tcpu->address_register = 0;
    tcpu->data_register    = 0;
    tcpu->interrupt_vector = 0;
    tcpu->memory           = get_memory();
    memset(&tcpu->spr,0,sizeof(tcpu->spr));
    memset(&tcpu->gpr,0,sizeof(tcpu->gpr));
    exit_if_ptr_is_null(tcpu,"Init alloc CPU error\n");
    return tcpu;
}
cpucore_t * get_cpu()
{
    static cpucore_t * cpu = NULL;

    if(cpu != NULL)
    {
        return cpu;
    }
    cpu = init_cpu();

    return cpu;

}
static inline uint32_t va_to_pa(cpucore_t * cpu,uint32_t va)
{
    
    if(cpu->spr[CURRENT_STATE] & PROTECTED_MODE)
    {
        uint32_t page_direct = cpu->spr[PDTR] + PAGE_DIRECT(va) << PAGE_ALIGN;
        uint32_t page_table;
        read_memory_32(cpu->memory,page_direct,&page_table);        
        if(!PAGE_PRESENT(page_table))
        {
            /* 缺页异常 */
            I_SET(cpu);
            cpu->spr[PAGE_FALUT_A] = page_direct;
            cpu->interrupt_vector = I_PAGE_FAULT;
            return 0;
        }
        uint32_t page_t_shif = page_table + PAGE_TABLE(va) << PAGE_ALIGN;
        uint32_t page_a;
        read_memory_32(cpu->memory,page_t_shif,&page_a);
        if(!PAGE_PRESENT(page_a))
        {
            /* 缺页异常 */
            I_SET(cpu);
            cpu->spr[PAGE_FALUT_A] = page_table;
            cpu->interrupt_vector  = I_PAGE_FAULT;
            return 0;
        }
        return page_a + PAGE_P_SHIFT(va);
    }
    else
    {
        return va;
    }
}

static inline uint8_t read_keyboard(cpucore_t * tcpu)
{
    uint32_t read_pos;
    read_memory_32(tcpu->memory,KEY_MMIO_ADDR+4,&read_pos);
    uint8_t c; 
    read_memory_8(tcpu->memory,KEY_MMAP_ADDR+read_pos,&c);
    ++read_pos;
    if(read_pos >= BUFFER_SIZE)
    {
        read_pos = 0;
    }
    write_memory_32(tcpu->memory,KEY_MMIO_ADDR+4,read_pos);
    return c;
}
void wait_screen_free(cpucore_t * tcpu)
{
    while(1)
    {
        uint32_t sm_flag; 
        read_memory_32(tcpu->memory,SCREEN_MMIO_ADDR,&sm_flag);
        if(sm_flag == 0)
        {
            break;
        }
    }
}
void * cpu_main_loop(void * arg)
{
    cpucore_t * tcpu = (cpucore_t *)arg;

    uint8_t c;
    int i = 0;
    while(1)
    {
        if(I_HAPPEN(tcpu))
        {
            I_CLEAR(tcpu);
            c = read_keyboard(tcpu);
            wait_screen_free(tcpu);
            write_memory_8(tcpu->memory,SCREEN_MMAP_ADDR+i,c);
            i++;
            write_memory_32(tcpu->memory,SCREEN_MMIO_ADDR,1);
        }
        else
        {
            usleep(10*2);
        }
    }

}


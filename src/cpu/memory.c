/*************************************************************************
	> File Name: memory.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 01:48:13 PM CST
 ************************************************************************/

#include "includes.h"


void exit_if_ptr_is_null(void * ptr,const char * message) 
{
    if(ptr == NULL)
    {   
        printf("%s\n",message);
        exit(-1);
    } 
}
/*
 * littel duan
 * size is MB uint
 * */
memory_t * init_memory(uint32_t size)
{
    memory_t * m = malloc(sizeof(memory_t));
    exit_if_ptr_is_null(m,"Error: when alloc memory struct");
    m->size = size * MB_SIZE;
    m->memory = calloc(m->size,1);
    exit_if_ptr_is_null(m->memory,"Error: when alloc memory_t memory");
    return m;
}
#define EXIT_IF_ADDR_BIGGER(addr,size) do \
{\
    if(addr > size)\
    {\
        printf("Error: read addr is BIGGER than memory size %u > %u\n",addr,size);\
        sleep(10);\
        endwin();\
        exit(-1);\
    }\
}while(0)
uint8_t read_memory_8(memory_t * m, uint32_t addr)
{
    EXIT_IF_ADDR_BIGGER(addr,m->size-1);
    return m->memory[addr];
}

void write_memory_8(memory_t * m, uint32_t addr,uint8_t  value)
{
    EXIT_IF_ADDR_BIGGER(addr,m->size-1);
    m->memory[addr] = value;
}


uint16_t read_memory_16(memory_t * m, uint32_t addr)
{
    EXIT_IF_ADDR_BIGGER(addr,m->size-2);
    return *(uint16_t *)&m->memory[addr];
}

void write_memory_16(memory_t * m, uint32_t addr,uint16_t  value)
{
    EXIT_IF_ADDR_BIGGER(addr,m->size-2);
    *(uint16_t *)&m->memory[addr] = value;
}


uint32_t read_memory_32(memory_t * m, uint32_t addr)
{ 
    EXIT_IF_ADDR_BIGGER(addr,m->size-4);
    return *(uint32_t *)&m->memory[addr];
} 

void write_memory_32(memory_t * m, uint32_t addr,uint32_t  value)
{
    EXIT_IF_ADDR_BIGGER(addr,m->size-4);
    *(uint32_t *)&m->memory[addr] = value;
}


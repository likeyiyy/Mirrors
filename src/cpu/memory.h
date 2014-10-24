/*************************************************************************
	> File Name: memory.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 01:48:19 PM CST
 ************************************************************************/

#ifndef L_MEMORY_H
#define L_MEMORY_H

#define MB_SIZE 1024*1024
typedef struct _memory
{
    uint8_t * memory;
    uint32_t size;
}memory_t;

memory_t * init_memory(uint32_t size);

void write_memory_8(memory_t * m,uint32_t addr,uint8_t value);

uint8_t read_memory_8(memory_t * m, uint32_t addr);


void write_memory_16(memory_t * m, uint32_t addr,uint16_t  value);
uint16_t read_memory_16(memory_t * m, uint32_t addr);


void write_memory_32(memory_t * m, uint32_t addr,uint32_t  value);
uint32_t read_memory_32(memory_t * m, uint32_t addr);


#endif

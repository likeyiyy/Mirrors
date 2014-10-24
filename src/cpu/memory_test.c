/*************************************************************************
	> File Name: memory_test.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 02:54:11 PM CST
 ************************************************************************/

#include "includes.h"

int main(int argc, char ** argv)
{
    memory_t * m = get_memory();
    for(int i = 0; i * 4< m->size; ++i)
    {
        write_memory_32(m,i*4,i%256);
    }

    uint8_t value;
    for(int i = 0; i < m->size; ++i)
    {
        if(((i) % 16 == 0))
        {
            printf("\n");
            printf("0x%08X:",i);
        }
        read_memory_8(m,i,&value);
        printf("%02x ",value);
    }
    printf("\n");
    uint16_t value16;
    for(int i = 0; i < m->size; i+=2)
    {
        if(((i) % 16 == 0))
        {
            printf("\n");
            printf("0x%08X:",i);
        }
        read_memory_16(m,i,&value16);
        printf("%04x ",value16);
    }
    printf("\n");
    uint32_t value32;
    for(int i = 0; i < m->size; i+=4)
    {
        if(((i) % 16 == 0))
        {
            printf("\n");
            printf("0x%08X:",i);
        }
        read_memory_32(m,i,&value32);
        printf("%08x ",value32);
    }
    printf("\n");
}


/*************************************************************************
	> File Name: mirriros.c
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Oct 2014 10:58:39 PM CST
 ************************************************************************/

#include "includes.h"
int main(int argc,char ** argv)
{
    memory_t * memory = init_memory(64);
    cpu_t * cpu = init_cpu(0x1234ABCD,0x7c01,memory);
    sys_display(cpu);
}

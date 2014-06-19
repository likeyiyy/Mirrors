/*************************************************************************
	> File Name: keyboard.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 03:38:33 PM CST
 ************************************************************************/

#ifndef L_KEYBOARD_H
#define L_KEYBOARD_H

#define BUFFER_SIZE 4096
typedef struct _keyboard
{
    cpucore_t * cpu;
    memory_t  * memory;
    uint32_t  buffer_size;
    uint32_t  mmio_start;
    uint32_t  flag_reg;
    uint32_t  head_reg;
    uint32_t  tail_reg;
    pthread_t id;
}keyboard_t;

/*
* Init keyboard
* 1.映射IO
* */

/*
* Loop 
* 1.read some thing
* 2.write to buffer
* */
#endif

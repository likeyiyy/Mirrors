/*************************************************************************
	> File Name: keyboard.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 03:38:33 PM CST
 ************************************************************************/

#ifndef L_KEYBOARD_H
#define L_KEYBOARD_H

typedef struct _keyboard
{
    cpucore_t * cpu;
    memory_t  * memory;
    uint32_t  buffer_size;
    uint32_t  mmio_start;
    uint32_t  mmap_start;
    uint32_t  write_pos_reg;
    uint32_t  read_pos_reg;
    pthread_t id;
}keyboard_t;

extern keyboard_t * keyboard;
/*
* Init keyboard
* 1.映射IO
* */
keyboard_t * init_keyboard(void);
/*
* Loop 
* 1.read some thing
* 2.write to buffer
* */
void * keyboard_loop(void * arg);
#endif

/*************************************************************************
	> File Name: screen.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 05:53:12 PM CST
 ************************************************************************/

#ifndef L_SCREEN_H
#define L_SCREEN_H

typedef struct _screen
{
    uint32_t screen_mmio_start;
    uint32_t screen_mmap_start;
    uint32_t buffer_size;
    uint32_t m_flag;
    memory_t * memory;
}screen_t;

screen_t * get_screen(void);

void * screen_main_loop(void * arg);
#endif

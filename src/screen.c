/*************************************************************************
	> File Name: screen.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 05:53:18 PM CST
 ************************************************************************/

#include "includes.h"

screen_t * init_screen(void)
{
    screen_t * s = malloc(sizeof(screen_t));
    exit_if_ptr_is_null(s,"Init screen error:alloc");
    s->screen_mmio_start = SCREEN_MMIO_ADDR;
    s->screen_mmap_start = SCREEN_MMAP_ADDR;
    s->buffer_size       = SCREEN_BUFFER_SIZE;
    s->m_flag            = 0;
    s->memory            = get_memory();

    return s;

}

screen_t * get_screen(void)
{
    static screen_t * s = NULL;
    if(s != NULL)
    {
        return s;
    }

    s = init_screen();

    return s;
}
static void write_screen(screen_t * s)
{
    uint8_t c;
    system("clear");
    for(int i = 0; i < s->buffer_size; ++i)
    {
        if(i % SCREEN_WIDTH == 0)
        {
            printf("\n");
        }
        
        read_memory_8(s->memory,SCREEN_MMAP_ADDR+i,&c);
        if(c == 0)
        printf("+"); 
        else
        printf("%c",c);
    }

}
void * screen_main_loop(void * arg)
{
    screen_t * s = (screen_t *)arg;
    while(1)
    {
        read_memory_32(s->memory,SCREEN_MMIO_ADDR,&s->m_flag);
        if(1 == s->m_flag)
        {
            write_screen(s);
            s->m_flag = 0;
            write_memory_32(s->memory,SCREEN_MMIO_ADDR,s->m_flag);
        }
        
    }
}















































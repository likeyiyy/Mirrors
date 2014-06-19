/*************************************************************************
	> File Name: test_key.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 05:07:19 PM CST
 ************************************************************************/

#include "includes.h"


int main(int argc,char ** argv)
{
    pthread_t wid,rid;
    pthread_t screenid;

    memory_t * memory = get_memory();
    cpucore_t * cpu = get_cpu();
    keyboard_t * keyboard = get_keyboard();
    screen_t   * screen = get_screen();

    pthread_create(&rid,NULL,cpu_main_loop,cpu);
    pthread_create(&wid,NULL,keyboard_loop,keyboard);
    pthread_create(&screenid,NULL,screen_main_loop,screen);

    while(1)
    {
        sleep(10);
    }
}




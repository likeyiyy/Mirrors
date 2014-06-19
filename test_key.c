/*************************************************************************
	> File Name: test_key.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 05:07:19 PM CST
 ************************************************************************/

#include "includes.h"

extern cpucore_t * cpu;
extern memory_t  * memory;
extern keyboard_t * keyboard;

int main(int argc,char ** argv)
{
    pthread_t wid,rid;

    init_memory(16);
    init_cpu();
    init_keyboard();

    pthread_create(&rid,NULL,cpu_main_loop,cpu);
    pthread_create(&wid,NULL,keyboard_loop,keyboard);

    while(1)
    {
        sleep(10);
    }
}




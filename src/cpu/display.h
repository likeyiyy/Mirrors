/*************************************************************************
	> File Name: display.h
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 10 Jun 2014 10:00:01 PM CST
 ************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

/*
* Fuction: Display the queue length pool length and so on
* Author:  likeyi
* Date:    2014年6月10日13:55:59
* */
typedef struct 
{
    int height;
    int width;
    int startx;
    int starty;
    WINDOW * win;
    cpu_t * cpu;
    pthread_mutex_t * lock;
}window_t;
window_t * init_screen(cpu_t * cpu,pthread_mutex_t * lock);
void * mem_console_main_loop(void * arg);
void * reg_console_main_loop(void * arg);
void * user_console_main_loop(void * arg);
#endif

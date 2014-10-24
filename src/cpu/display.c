/*************************************************************************
	> File Name: dispaly.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 10 Jun 2014 10:05:03 PM CST
 ************************************************************************/

#include "includes.h"
#include <ncurses.h>
#define WORKER_SET_SIZE 3

#define USER_CONSOLE_HIGH  3
#define USER_CONSOLE_WIDTH COLS
#define USER_CONSOLE_STARTX 0
#define USER_CONSOLE_STARTY (LINES - USER_CONSOLE_HIGH )

#define MEMORY_CONSOLE_HIGH (LINES - USER_CONSOLE_HIGH )
#define MEMORY_CONSOLE_WIDTH (COLS / 2)
#define MEMORY_CONSOLE_STARTX 0
#define MEMORY_CONSOLE_STARTY 0

#define REG_CONSOLE_HIGH (LINES - USER_CONSOLE_HIGH )
#define REG_CONSOLE_WIDTH  (COLS / 2)
#define REG_CONSOLE_STARTX (COLS / 2)
#define REG_CONSOLE_STARTY 0

typedef struct 
{
    int height;
    int width;
    int startx;
    int starty;
    WINDOW * win;
}window_t;
static window_t window[WORKER_SET_SIZE];
static void screen_init()
{
    initscr();
    window[0].height = MEMORY_CONSOLE_HIGH;
    window[0].width  = MEMORY_CONSOLE_WIDTH;
    window[0].starty = MEMORY_CONSOLE_STARTY;
    window[0].startx = MEMORY_CONSOLE_STARTX;
    window[0].win    = newwin(
               window[0].height,
               window[0].width,
               window[0].starty,
               window[0].startx
                );
    //scrollok(window[0].win,1);
    window[1].height = REG_CONSOLE_HIGH;
    window[1].width  = REG_CONSOLE_WIDTH;
    window[1].starty = REG_CONSOLE_STARTY;
    window[1].startx = REG_CONSOLE_STARTX;
    window[1].win    = newwin(
               window[1].height,
               window[1].width,
               window[1].starty,
               window[1].startx
                );
    //scrollok(window[1].win,1);
    window[2].height = USER_CONSOLE_HIGH;
    window[2].width  = USER_CONSOLE_WIDTH;
    window[2].starty = USER_CONSOLE_STARTY;
    window[2].startx = USER_CONSOLE_STARTX;
    window[2].win    = newwin(
               window[2].height,
               window[2].width,
               window[2].starty,
               window[2].startx
                );
    scrollok(window[2].win,1);
}
static void print_memory(window_t * win,cpu_t * cpu)
{
    int high = win->height;
    uint32_t start_addr = cpu->PC - high/2;
    uint32_t end_addr   = cpu->PC + high/2;

    
    for(uint32_t i = start_addr; i < end_addr; i++)
    {
        wprintw(win->win,"0x%08x   %08X  \n",i,read_memory_32(cpu->memory,i));
    }
    wrefresh(win->win);
}
static void print_cpu(window_t * win,cpu_t * cpu)
{
    wprintw(win->win,"CPUID   0x%X\n",cpu->CPUID);
    wprintw(win->win,"PC      0x%X\n",cpu->PC);
    wprintw(win->win,"PWD     0x%X\n",cpu->PWD);
    for(int i = 0; i < 16; i++)
    {
        wprintw(win->win,"reg[%02d] 0x%08X %010u\t",i,cpu->reg[i],cpu->reg[i]);
        wprintw(win->win,"reg[%02d] 0x%08X %010u\n",i+16,cpu->reg[i+16],cpu->reg[i+16]);
    }
    wrefresh(win->win);
}
static void print_user(window_t * win,cpu_t * cpu)
{
    char buf[20];
    wprintw(win->win,">");
    wrefresh(win->win);
    while(1)
    {
        wscanw(win->win,"%s",buf);
        if(strcmp("quit",buf) == 0)
        {
            endwin();
            exit(0);
        }
        else if(strcmp("some",buf) == 0)
        {
            wprintw(win->win,"%s input\n",buf);
            wprintw(win->win,">");
        }
        else
        {
            wprintw(win->win,"%s not such command\n",buf);
            wprintw(win->win,">");
        }
        wrefresh(win->win);
    }
}
/*
* Fuction: Display the queue length pool length and so on
* Author:  likeyi
* Date:    2014年6月10日13:55:59
* */
void sys_display(cpu_t * cpu)
{
    screen_init();
    start_color();
    print_memory(&window[0],cpu);
    print_cpu(&window[1],cpu);
    print_user(&window[2],cpu);
    endwin();
}

/*************************************************************************
	> File Name: keyboard.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 03:38:27 PM CST
 ************************************************************************/

#include "includes.h"

extern cpucore_t * cpu;
extern memory_t  * memory;
keyboard_t * keyboard;
static int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;

    }
    return 0;
}
/*
* Init keyboard
* 1.映射IO
* */
keyboard_t * init_keyboard(void)
{
    keyboard_t * key = malloc(sizeof(keyboard_t));
    key->cpu    = cpu;
    key->memory = memory;
    key->buffer_size = BUFFER_SIZE + 8;
    key->mmio_start  =  KEY_MMIO_ADDR;
    key->mmap_start  =  KEY_MMAP_ADDR;
    key->write_pos_reg = 0;
    key->read_pos_reg  = 0;

    keyboard = key;
    return key;
}
/*
* Loop 
* 1.read some thing
* 2.write to buffer
* */
void * keyboard_loop(void * arg)
{
    keyboard_t * key = (keyboard_t *) arg;

    while(1)
    {
        if(kbhit())
        {
            uint8_t c = getchar(); 
            write_memory_8(key->memory,key->mmap_start+key->write_pos_reg,c);
            ++key->write_pos_reg;
            if(key->write_pos_reg >= BUFFER_SIZE)
            {
                key->write_pos_reg = 0;
            }
            write_memory_32(key->memory,key->mmio_start,key->write_pos_reg);
            key->cpu->interrupt_flag = 1;
            key->cpu->interrupt_vector = KEY_INTERRUPT_VECTOR;
        }
        else
        {
            usleep(1);
        }
    }

}

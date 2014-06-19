/*************************************************************************
	> File Name: includes.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 01:51:10 PM CST
 ************************************************************************/

#ifndef L_INCLUDES_H
#define L_INCLUDES_H

#define KEY_INTERRUPT_VECTOR 0x0

#define KEY_MMIO_ADDR 0x100000
#define KEY_MMAP_ADDR 0x100008
#define BUFFER_SIZE 1024

#define SCREEN_WIDTH (80 * 1)
#define SCREEN_HIGH  (25 * 2)
#define SCREEN_MMIO_ADDR (KEY_MMAP_ADDR + BUFFER_SIZE) 
#define SCREEN_MMAP_ADDR SCREEN_MMIO_ADDR+4
#define SCREEN_BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HIGH)

#define MMIO_START KEY_MMIO_ADDR
#define MMIO_END   SCREEN_MMAP_ADDR+SCREEN_BUFFER_SIZE

/*
 * MB
 * */
#define MEMORY_SIZE 16


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
/*
 * Common functional
 * */
static inline void exit_if_ptr_is_null(void * ptr,char * str)
{
    if(ptr == NULL)
    {
        printf("%s\n",str);
        exit(-1);
    }
}
/*
*  Common data struct
* */


/*
* Common header file
* */
#include "memory.h"
#include "cpucore.h"
#include "keyboard.h"
#include "screen.h"
#endif

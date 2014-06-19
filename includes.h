/*************************************************************************
	> File Name: includes.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 19 Jun 2014 01:51:10 PM CST
 ************************************************************************/

#ifndef L_INCLUDES_H
#define L_INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
#include "cpu.h"
#include "memory.h"
#include "keyboard.h"
#endif

/*************************************************************************
	> File Name: cpucore.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Tue 06 May 2014 05:57:59 PM CST
 ************************************************************************/
#ifndef CPUCORE_H
#define CPUCORE_H
typedef struct _cpucore
{

    uint32_t address_register;
    uint32_t data_register;
    uint32_t interrupt_vector;
    int      interrupt_flag;
}cpucore_t;

#endif

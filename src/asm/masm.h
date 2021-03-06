/*************************************************************************
	> File Name: masm.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Fri 24 Oct 2014 03:27:06 PM CST
 ************************************************************************/

#ifndef __MASM_H
#define __MASM_H

/* Total 32       */
/* 1. 算数指令  6 */
#define ADD		0
#define ADDI	1
#define SUB		2
#define MUL		3
#define DIV		4
#define MOD		5
/* 2. 逻辑指令  7 */
#define AND		6
#define OR		7
#define NOT		8
#define XOR	    9
#define ANDI	10
#define ORI		11
#define XORI	12
/* 3. 移位指令  4 */
#define SLL		13
#define SLR		14
#define SAL		15
#define SAR		16
/* 4. 转移指令  11 */
#define LESS	17
#define GREAT	18
#define LESSE	19
#define GREATE	20

#define LESSU	21
#define GREATU	22
#define LESSEU	23
#define GREATEU	24

#define EQUAL	25
#define UEQUAL	26
#define JMP		27
/* 5. 存储指令  4 */
#define MOV		28
#define LWORD	29
#define SWORD	30
#define LUI		31

#endif

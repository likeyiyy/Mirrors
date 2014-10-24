/*************************************************************************
	> File Name: cpucpu->c
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Oct 2014 09:44:32 PM CST
 ************************************************************************/

#include "includes.h"
static inline int32_t signedimm(uint32_t instruction)
{
    unsigned short imm = instruction & 0xffff;
    if(imm&0x8000)
    {
        return (int)imm|0xffff0000;
    }
    else
    {
        return (int)imm&0x0000ffff;
    }
}
static inline uint32_t zeroeximm(uint32_t instruction)
{
    return instruction & 0x0000ffff;
}
static uint32_t getfetch(cpu_t * cpu)
{
    uint32_t instruction = read_memory_32(cpu->memory,cpu->PC);
    cpu->PC += 4;
    return instruction;
}
static void exectued(cpu_t * cpu, uint32_t in)
{
    uint32_t getmask = 0;
	switch(in >> 26)
	{
	case ADD:
		RD(in) = RS(in) + RT(in);
		break;
	case ADDI:
		RD(in) = RS(in) + signedimm(in);
		break;
	case SUB:
		RD(in) = RS(in) - RT(in);
		break;
	case MUL:
		RD(in) = RS(in) * RT(in);
		break;
	case DIV:
	    if(RT(in) == 0)
        {
            cpu->PWD &= 1;
        }
        else
        {
            RD(in) = RS(in) / RT(in);
        }
		break;
	case MOD:
	    if(RT(in) == 0)
        {
            cpu->PWD &= 1 << 1;
        }
        else
        {
		    RD(in) = RS(in) % RT(in);
        }
		break;
	case AND:
		RD(in) = RS(in) & RT(in);
		break;
	case OR:
		RD(in) = RS(in) | RT(in);
		break;
	case NOT:
		RD(in) = ~RS(in);
		break;
	case XOR:
		RD(in) = RS(in) ^ RT(in);
		break;
	case ANDI:  ///这里的立即数是0扩展的。
		RD(in) = RS(in) & zeroeximm(in);
		break;
	case ORI:
		RD(in) = RS(in) | zeroeximm(in);
		break;
	case XORI:
		RD(in) = RS(in) ^ zeroeximm(in);
		break;
    ///C语言中的左移就是逻辑左移，而右移，
    ///对于无符号数来说就是逻辑右移，
    ///对有符号来说就是算术右移
    ///想要实现符号左移，比较麻烦一点，首先保存最高位，然后左移之后补上最高位。
	case SLL:
		RD(in) = RS(in) << zeroeximm(in);
		break;
	case SLR:
		RD(in) = RS(in) >> zeroeximm(in);
		break;
    case SAL:
        getmask = RS(in)&0x80000000;
		RD(in) = RS(in) << zeroeximm(in);
		RD(in) |= getmask;
		break;
	case SAR:
		RD(in) = (signed)RS(in) >> zeroeximm(in);
		break;
    /*跳转指令*/
	case LESS:
		if ((signed)RD(in) < (signed)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case GREAT:
		if ((signed)RD(in) > (signed)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
    case LESSE:
		if ((signed)RD(in) <= (signed)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case GREATE:
		if ((signed)RD(in) >= (signed)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case LESSU:
		if ((unsigned)RD(in) < (unsigned)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp ;     ///然后相对PC偏移
		}
		break;
	case GREATU:
		if ((unsigned)RD(in) > (unsigned)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
    case LESSEU:
		if ((unsigned)RD(in) <= (unsigned)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case GREATEU:
		if ((unsigned)RD(in) >= (unsigned)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case EQUAL:
		if ((unsigned)RD(in) == (unsigned)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case UEQUAL:
		if ((unsigned)RD(in) != (unsigned)RS(in))
		{
			int tmp = signedimm(in);
			cpu->PC -= 4;           ///PC值首先减四。
			cpu->PC += tmp;     ///然后相对PC偏移
		}
		break;
	case JMP:
		cpu->PC = (uint32_t)RD(in);
		break;
    /* 存储指令 */
	case MOV:
		RD(in) = RS(in);
		break;
	case LWORD:
		RD(in) = read_memory_32(cpu->memory,RS(in)+RT(in));
		break;
	case SWORD:
        write_memory_32(cpu->memory,RS(in)+RT(in),RD(in));
		break;
	case LUI: ///哦，载入高16位数啊，靠，那么低位怎么载入呢？用ori
		RD(in) = (RD(in) & 0x0000ffff)|(zeroeximm(in)<<16);
		break;
    default:
        cpu->PWD &= 1 << 2;
        break;
	}
}
void * cpu_main_loop(void * arg)
{
    cpu_t * cpu = (cpu_t *)arg;
    /*1. get instruction    */
    /*2. exectue instruction*/
    while(1)
    {
        exectued(cpu,getfetch(cpu));
    }
}
cpu_t * init_cpu(uint32_t cpuid,uint32_t pc,memory_t * memory)
{
    cpu_t * cpu = calloc(1,sizeof(cpu_t));
    if(cpu == NULL)
    {
        printf("init cpu error\n");
        exit(0);
    }
    cpu->CPUID = cpuid;
    cpu->PC = pc;
    cpu->memory = memory;
    return cpu;
}

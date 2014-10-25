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
static inline int32_t signedjmpimm(uint32_t instruction)
{
    unsigned short imm = instruction & 0x03fffffff;
    if(imm&0x2000000)
    {
        return (int)imm|0xff000000;
    }
    else
    {
        return (int)imm&0x03ffffff;
    }
}
static inline uint32_t zeroeximm(uint32_t instruction)
{
    return instruction & 0x0000ffff;
}
static uint32_t getfetch(cpu_t * cpu)
{
    if(cpu->iPC > 64*1024*1024 - 4)
    {
        cpu->iPC = 0;
    }
    cpu->PC = cpu->iPC;
    uint32_t instruction = read_memory_32(cpu->memory,cpu->PC);
    cpu->iPC += 4;
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
            if((cpu->iPC + (tmp * 4)) <= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case GREAT:
        if ((signed)RD(in) > (signed)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4)) <= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case LESSE:
        if ((signed)RD(in) <= (signed)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4)) <= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case GREATE:
        if ((signed)RD(in) >= (signed)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case LESSU:
        if ((unsigned)RD(in) < (unsigned)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case GREATU:
        if ((unsigned)RD(in) > (unsigned)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case LESSEU:
        if ((unsigned)RD(in) <= (unsigned)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case GREATEU:
        if ((unsigned)RD(in) >= (unsigned)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case EQUAL:
        if ((unsigned)RD(in) == (unsigned)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case UEQUAL:
        if ((unsigned)RD(in) != (unsigned)RS(in))
        {
            int tmp = signedimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    case JMP:
        {
            int tmp = signedjmpimm(in);
            if((cpu->iPC + (tmp * 4) )<= cpu->memory->size)
            {
                cpu->iPC -= 4;           ///PC值首先减四。
                cpu->iPC += tmp * 4;     ///然后相对PC偏移
            }
        }
        break;
    /* 存储指令 */
    case MOV:
        RD(in) = RS(in);
        break;
    case LWORD:
        if((RS(in) + RT(in))<= (cpu->memory->size -4))
            RD(in) = read_memory_32(cpu->memory,RS(in)+RT(in));
        break;
    case SWORD:
        if((RS(in) + RT(in))<= (cpu->memory->size -4))
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
static void code_generator(uint32_t * codes,int length)
{
    for(int i = 0; i < length; i++)
    {
        codes[i] = rand();
    }
}
static void code_collector(uint32_t * codes,int length)
{
    static int cur_size = 0;
    static code_t save_code[MAXSIZE];
    if(cur_size < MAXSIZE)
    {
        save_code[cur_size].codes  = codes;
        save_code[cur_size].length = length;
        cur_size++;
    }
    else
    {
        //printf("save enough, You can Quit\n");
    }
}
static void clone_cpu(cpu_t * cpu1,cpu_t * cpu2)
{
    cpu1->iPC = cpu2->iPC;
    cpu1->PC  = cpu2->PC;
    cpu1->PWD = cpu2->PWD;
    //printf("$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    memcpy(cpu1->reg,cpu2->reg,sizeof(cpu2->reg));
}
void * cpu_main_loop(void * arg)
{
    mirror_t * mirror = (mirror_t *)arg;
    cpu_t * imaginary = mirror->imaginary;
    cpu_t * real      = mirror->real;
    uint32_t * codes;
    uint32_t current = 0;
    int32_t old_flag,new_flag;
    old_flag = new_flag = 0;
    while(1)
    {
        codes = malloc(CODE_LENGTH * 4);
        /*1. 产生代码 */
        code_generator(codes,CODE_LENGTH);
        //printf("PC:%d %d\n",real->PC,imaginary->PC);
        memcpy(imaginary->memory->memory + imaginary->PC, codes, CODE_LENGTH * 4);
        memcpy(real->memory->memory + real->PC, codes, CODE_LENGTH * 4);
        /*2. 交给虚拟处理器处理 */
        for(int i = 0; i < CODE_LENGTH * 2; i++)
        {
            exectued(imaginary,getfetch(imaginary));
        }
        /*3. 获得结果     */
        new_flag = read_memory_32(imaginary->memory, FLAG_ADDRES);
        /*4. 真实的CPU是否可以执行 */
        if(new_flag >= old_flag)
        {
            old_flag = new_flag;
            mirror->success_test++;
            //可以执行，执行。
            for(int i = 0; i < CODE_LENGTH * 2; i++)
            {
                exectued(real,getfetch(real));
            }
            code_collector(codes,CODE_LENGTH);
            if((real->PC + CODE_LENGTH * 4 + 4) >= imaginary->memory->size)
            {
                imaginary->PC = 0;
                real->PC = 0;
            }
        }
        else
        {
            mirror->failure_test++;
            free(codes);
            clone_cpu(imaginary,real);
        }
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
    cpu->iPC = pc;
    cpu->memory = memory;
    return cpu;
}

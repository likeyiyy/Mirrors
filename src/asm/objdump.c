/*************************************************************************
    > File Name: objdump.c
    > Author: 
    > Mail: 
    > Created Time: Fri 24 Oct 2014 11:50:05 PM CST
 ************************************************************************/

#include "includes.h"

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
char * opcodes[32] = {
    "add","addi","sub","mul","div","mod",                //6
    "and","or","not","xor","andi","ori","xori",          //7
    "sll","slr","sal","sar",                             //4
    "less","great","lesse","greate","lessu",             //5
    "greatu","lesseu","greateu","equal","uequal","jmp",  //6
    "mov","lword","sword","lui"                          //4
                   };
char * reg_name[32] = {
    "r0","r1","r2","r3","r4","r5","r6","r7","r8","r9",
    "r10","r11","r12","r13","r14","r15","r16","r17","r18","r19",
    "r20","r22","r22","r23","r24","r25","r26","r27","r28","r29",
    "r30","r31"
};
static inline char * get_opcode_desc(int opcode)
{
    return opcodes[opcode];
}
static inline char * get_reg_desc(int reg)
{
    return reg_name[reg];
}
#define RD(n) ((n>>21)&0x1f)
#define RS(n) ((n>>16)&0x1f)
#define RT(n) ((n>>11)&0x1f)
char * get_deassembly(uint32_t in)
{
    static char buf[256];
    int16_t imm;
    int32_t jmpimm;
    switch(in >> 26)
    {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
        case AND:
        case OR:
        case NOT:
        case XOR:
        case LWORD:
        case SWORD:
        sprintf(buf,"%-6s %s, %s, %s",
                get_opcode_desc(in >> 26),
                get_reg_desc(RD(in)),
                get_reg_desc(RS(in)),
                get_reg_desc(RT(in))
               );
        return buf;
        break;
        case SLL:
        case SLR:
        case SAL:
        case SAR:
        case ADDI:
        case ANDI:  ///这里的立即数是0扩展的。
        case ORI:
        case XORI:
        case LUI: ///哦，载入高16位数啊，靠，那么低位怎么载入呢？用ori
        case LESS:
        case GREAT:
        case LESSE:
        case GREATE:
        case LESSU:
        case GREATU:
        case LESSEU:
        case GREATEU:
        case EQUAL:
        case UEQUAL:
        imm = in & 0x0000ffff;
        sprintf(buf,"%-6s %s, %s, %d",
                get_opcode_desc(in >> 26),
                get_reg_desc(RD(in)),
                get_reg_desc(RS(in)),
                imm
               );
        return buf;
        break;
        case JMP:
        jmpimm = signedjmpimm(in);
        sprintf(buf,"%-6s %d",
                get_opcode_desc(in >> 26),
                jmpimm);
        return buf;
        break;
        case MOV:
        sprintf(buf,"%-6s %s, %s",
                get_opcode_desc(in >> 26),
                get_reg_desc(RD(in)),
                get_reg_desc(RS(in))
               );
        return buf;
        break;
        default:
            return "Invaild instruction";
            break;
    }
}
int main(int argc,char ** argv)
{
    if(argc != 3)
    {
        printf("objdump -o obj_file\n");
        exit(-1);
    }
    if(strcmp("-o",argv[1]) != 0)
    {
        printf("objdump -o obj_file\n");
        exit(-1);
    }
    char * obj_name = argv[2];
    FILE * fp;
    if((fp = fopen(obj_name,"r")) == NULL)
    {
        printf("Can not open %s : %s\n",obj_name,strerror(errno));
        exit(-1);
    }
    int counter = 0;
    uint32_t instruction;
    char buf[128] = {0}; 
    while(1)
    {
        if(fread(&instruction,sizeof(uint32_t),1,fp) == 1)
        {
            printf("%08x 0x%08x %s\n",counter,instruction,get_deassembly(instruction));
        }
        else if(feof(fp))
        {
            break;
        }
        else
        {
            printf("read %s error: %s\n",obj_name,strerror(errno));
            exit(-1);
        }
        counter++;
    }
    
}

/*************************************************************************
	> File Name: dispaly.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 10 Jun 2014 10:05:03 PM CST
 ************************************************************************/

#include "includes.h"
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
#define RDI(n) ((n>>21)&0x1f)
#define RSI(n) ((n>>16)&0x1f)
#define RTI(n) ((n>>11)&0x1f)
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
                get_reg_desc(RDI(in)),
                get_reg_desc(RSI(in)),
                get_reg_desc(RTI(in))
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
                get_reg_desc(RDI(in)),
                get_reg_desc(RSI(in)),
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
                get_reg_desc(RDI(in)),
                get_reg_desc(RSI(in))
               );
        return buf;
        break;
        default:
            return "Invaild instruction";
            break;
    }
}
window_t * init_screen(mirror_t * mirror,pthread_mutex_t * lock)
{
    initscr();
    window_t * window = malloc(sizeof(window_t) * 3);
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
    scrollok(window[0].win,1);
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
    scrollok(window[1].win,1);
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
    window[0].mirror = mirror;
    window[1].mirror = mirror;
    window[2].mirror = mirror;
    window[0].lock = lock;
    window[1].lock = lock;
    window[2].lock = lock;
    return window;
}
void * mem_console_main_loop(void * arg)
{
    window_t * win = (window_t *)arg;
    cpu_t * cpu = win->mirror->real;
    int high = win->height;
    uint32_t oldpc = 0;
    uint32_t start_addr,end_addr;
    start_addr = end_addr = 0;
    while(1)
    {
        if(oldpc == cpu->PC)
        {
            continue;
        }
        pthread_mutex_lock(win->lock);
        wclear(win->win);
        if(cpu->PC > end_addr || cpu->PC < start_addr)
        {
            start_addr = cpu->PC - high/2 * 4 ;
            end_addr   = cpu->PC + high/2 * 4;
        }
        uint32_t instruction;
        for(uint32_t i = start_addr; i < end_addr ; i+=4)
        {

            instruction = read_memory_32(cpu->memory,i);
            if(i == cpu->PC)
            {
                wprintw(win->win,">0x%08x   %08X  %s\n",i,instruction,get_deassembly(instruction));
            }
            else
            {
                wprintw(win->win," 0x%08x   %08X  %s\n",i,instruction,get_deassembly(instruction));
            }
        }
        wrefresh(win->win);
        oldpc = cpu->PC;
        pthread_mutex_unlock(win->lock);
    }
}
void * reg_console_main_loop(void * arg)
{
    window_t * win = (window_t *)arg;
    cpu_t * cpu = win->mirror->real;
    uint32_t oldpc = 0;
    while(1)
    {
        if(oldpc == cpu->PC)
        {
            continue;
        }
        pthread_mutex_lock(win->lock);
        wclear(win->win);
        wprintw(win->win,"CPUID   0x%08X\n",cpu->CPUID);
        wprintw(win->win,"PC      0x%08X %d\n",cpu->PC,cpu->PC);
        wprintw(win->win,"PWD     0x%08X\n",cpu->PWD);
        for(int i = 0; i < 16; i++)
        {
            wprintw(win->win,"reg[%02d] 0x%08X %010u\t",i,cpu->reg[i],cpu->reg[i]);
            wprintw(win->win,"reg[%02d] 0x%08X %010u\n",i+16,cpu->reg[i+16],cpu->reg[i+16]);
        }
        wrefresh(win->win);
        oldpc = cpu->PC;
        pthread_mutex_unlock(win->lock);
    }
}
void * user_console_main_loop(void * arg)
{
    char buf[20];
    window_t * win = (window_t *)arg;
    mirror_t * mirror = win->mirror;
    wprintw(win->win,">");
    wrefresh(win->win);
    uint64_t ftest,stest;
    ftest = stest = 0;
    while(1)
    {
        if((ftest != mirror->failure_test) || (stest != mirror->success_test))
        {
            ftest = mirror->failure_test;
            stest = mirror->success_test;
            wprintw(win->win,"Failure:%16lu,Success:%16lu\n",ftest,stest);
            wrefresh(win->win);
        }

        #if 0
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
        usleep(gsleep);
        #endif
    }
}

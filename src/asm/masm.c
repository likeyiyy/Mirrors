/*************************************************************************
	> File Name: masm.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Fri 24 Oct 2014 10:36:14 AM CST
 ************************************************************************/

#include "includes.h"

typedef struct
{
    char * name;
    int line;
}var_t;
static inline int line_has_label(const char * p)
{
    while(p[0] != '\0')
    {
        if(p[0] == ':')
        {
            return 1;
        }
        p++;
    }
    return 0;
}
static inline char * get_first_token(const char * p)
{
    while(!isspace(p[0]) && p[0] != ':' && p[0] != '\0')
    {
        p++;
    }
    return (char *)p;
}
static inline char * get_reg_token(const char * p)
{
    while(!isspace(p[0]) && p[0] != ',' && p[0] != '\0')
    {
        p++;
    }
    return (char *)p;
}
static inline char * get_opcode_token(const char * p)
{
    while(!isspace(p[0]) && p[0] != '\0')
    {
        p++;
    }
    return (char *)p;
}
static inline char * skip_label_wthie(const char * p)
{
    while(isspace(p[0]) || p[0] == ':' || p[0] == '\t')
    {
        p++;
    }
    return (char *)p;
} 
static inline char * skip_reg_wthie(const char * p)
{
    while(isspace(p[0]) || p[0] == ',' || p[0] == '\t')
    {
        p++;
    }
    return (char *)p;
} 
static inline char * skip_wthie(const char * p)
{
    while(isspace(p[0]) || p[0] == '\t')
    {
        p++;
    }
    return (char *)p;
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
void usage()
{
    printf("Usage masm [OPTIONS] files ...\n\n");
    printf("      masm -o obj_file src_file Masm format\n");
    printf("      masm --help               Help info\n");
}
static int verify_opcode(char * opcode,int lines)
{
    for(int i = 0; i < 32; i++)
    {
        if(!strcmp(opcodes[i],opcode))
        {
            return i;
        }
    }
    printf("[ERROR 3]:invalid instruction opcode line: %d\n",lines);
    exit(-3);
}
static inline int get_reg_index(char * reg,int line)
{
    for(int i = 0; i < 32; i++)
    {
        if(!strcmp(reg_name[i],reg))
        {
            return i;
        }
    }
    printf("[ERROR 4]:invalid reg num line: %d\n",line);
    exit(-4);
}
static inline char * skip_op_name(const char * p)
{
    while(!isspace(p[0]) && p[0] != ':' && p[0] != '\0')
    {
        p++;
    }
    return (char *)p;
}
uint32_t str2hash(char *str) 
{ 
    int i,l; 
    uint32_t ret=0; 
    uint16_t *s;
    if (str == NULL) 
    {
        return(0); 
    }
    l=(strlen(str)+1)/2; 
    s=(uint16_t *)str; 
    for (i=0; i<l;i++)
    {
        ret^=(s[i]<<(i&0x0f)); 
    }
    return(ret); 
}
static int get_file_lines(FILE * fp)
{
    int lines = 0;
    char buf[BUFSIZ];
    assert(fp != NULL);
    while(1)
    {
        fgets(buf,BUFSIZ,fp);
        if(feof(fp))
        {
            break;
        }
        lines++;
    }
    return lines;
}
static int masm_main_loop(char * obj_file,char * src_file)
{
    FILE * obj_fp, *src_fp;
    char buf[BUFSIZ] = {0};
    int     length;
    char *  p, * q;
    src_fp = obj_fp = NULL;
    uint32_t counter = 0;
    uint32_t lines   = 0;
    hash_table * label_hash = hash_create(512);

    char op_name[128];
    char label[128];
    char rd[6];
    char rs[6];
    char rt[6];
    char imm[20];
    int32_t rd_num,rs_num,rt_num,imm_num;

    if((src_fp = fopen(src_file,"r")) == NULL)
    {
        printf("Can not open %s:%s\n",src_file,strerror(errno));
    }
    if((obj_fp = fopen(obj_file,"w+")) == NULL)
    {
        printf("Can not open %s:%s\n",obj_file,strerror(errno));
    }
    int total_lines = get_file_lines(src_fp);
    uint32_t * instruction = calloc(1,total_lines * sizeof(uint32_t));
    var_t    * var = calloc(1,total_lines * sizeof(var_t));
    int     var_count = 0;
    fseek(src_fp,0L,SEEK_SET);
    while(1)
    {
        fgets(buf,BUFSIZ,src_fp);
        if(feof(src_fp))
        {
            break;
        }
        lines++;
        length = strlen(buf);
        p     = buf;
        
        //skip whitespace
        while(length > 0 &&isspace(p[0]))
        {
            length--;
            p++;
        }
        //printf("length=%d\t%s",length,buf+i);
        if(p[0] == ';' || p[0] == '\0')
        {
            continue;
        }
        q = get_first_token(p);
        strncpy(op_name, p , q-p);
        op_name[q-p] = '\0';
        if(line_has_label(p))
        {
            /* it is label */
            label_t l;
            l.name = op_name;
            l.real_line = lines;
            l.line = counter;
            hash_add_item(&label_hash,str2hash(op_name),&l);
            p = skip_label_wthie(q);
            /* 获得字符串 */
            q = get_opcode_token(p);
            strncpy(op_name, p , q-p);
            op_name[q-p] = '\0';
            //printf("%s",op_name);
        }

        /* p now a opcode start q-p is opecode */
        int op_index = verify_opcode(op_name,lines);
        q = skip_wthie(q);
        p = q;
        /* now at rd */

        switch(op_index)
        {
#if 1
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
                /* 获得字符串 */
            q = get_reg_token(p);
            strncpy(rd, p , q-p);
            rd[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(rs, p , q-p);
            rs[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(rt, p , q-p);
            rt[q-p] = '\0';
            rd_num = get_reg_index(rd,lines);
            rs_num = get_reg_index(rs,lines);
            rt_num = get_reg_index(rt,lines);
            instruction[counter] = (op_index << 26) | (rd_num << 21) | (rs_num << 16)| (rt_num << 11);
            break;
            ///C语言中的左移就是逻辑左移，而右移，
            ///对于无符号数来说就是逻辑右移，
            ///对有符号来说就是算术右移
            ///想要实现符号左移，比较麻烦一点，首先保存最高位，然后左移之后补上最高位。
        	case SLL:
        	case SLR:
            case SAL:
        	case SAR:
        	case ADDI:
        	case ANDI:  ///这里的立即数是0扩展的。
        	case ORI:
        	case XORI:
        	case LUI: ///哦，载入高16位数啊，靠，那么低位怎么载入呢？用ori
            q = get_reg_token(p);
            strncpy(rd, p , q-p);
            rd[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(rs, p , q-p);
            rs[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(imm, p , q-p);
            imm[q-p] = '\0';
            rd_num = get_reg_index(rd,lines);
            rs_num = get_reg_index(rs,lines);
            imm_num = atoi(imm);
            if(imm_num > 32767 || imm_num < -32768)
            {
                printf("________\n");
                printf("[ERROR 6] line: %d imm num is too lager or too smaller\n",lines);
            }
            instruction[counter] = (op_index << 26) | (rd_num << 21) | (rs_num << 16)| (imm_num & 0x0000ffff);

            break;
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
            q = get_reg_token(p);
            strncpy(rd, p , q-p);
            rd[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(rs, p , q-p);
            rs[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(label, p , q-p);
            label[q-p] = '\0';
            rd_num = get_reg_index(rd,lines);
            rs_num = get_reg_index(rs,lines);
            var[var_count].name = malloc(strlen(label) + 1);
            strcpy(var[var_count].name, label);
            var[var_count].line = counter;
            var_count++;
            instruction[counter] = (op_index << 26) | (rd_num << 21) | (rs_num << 16) | 0x0;
        	break;
        	case JMP:
            q = get_reg_token(p);
            strncpy(label, p , q-p);
            label[q-p] = '\0';
            var[var_count].name = malloc(strlen(label) + 1);
            strcpy(var[var_count].name, label);
            var[var_count].line = counter;
            var_count++;
            instruction[counter] = (op_index << 26);
            break;
            /* 存储指令 */
        	case MOV:
            q = get_reg_token(p);
            strncpy(rd, p , q-p);
            rd[q-p] = '\0';
            q = skip_reg_wthie(q);
            p = q;

            q = get_reg_token(p);
            strncpy(rs, p , q-p);
            rs[q-p] = '\0';
            rd_num = get_reg_index(rd,lines);
            rs_num = get_reg_index(rs,lines);
            instruction[counter] = (op_index << 26) | (rd_num << 21) | (rs_num << 16) | 0x0;
                break;
            default:
                break;
            #endif
        }
        counter++;
    }

    /* 第二趟汇编 */
    struct blist * head;
    for(int i = 0; i < var_count; i++)
    {
        if((head = hash_lookup_item(label_hash,str2hash(var[i].name),&var[i])) != NULL)
        {
            label_t * node = head->item;
            int imm_2 = node->line - var[i].line;
            if((instruction[var[i].line] >> 26) == JMP)
            {
                if(imm_2 > 33554431 || imm_2 < -33554432)
                {
                    printf("[ERROR 7] line: %d imm num is too lager or too smaller\n",lines);
                }
                instruction[var[i].line] |= imm_2 & 0x03ffffff;
            }
            else
            {
                if(imm_2 > 32767 || imm_2 < -32768)
                {
                    printf("[ERROR 6] line: %d imm num is too lager or too smaller\n",lines);
                }
                instruction[var[i].line] |= imm_2 & 0x0000ffff;
            }
        }
        else
        {
            printf("[ERROR 5]: line %d : %s not defined\n",var[i].line,var[i].name);
        }

    }
    fwrite(instruction,sizeof(uint32_t),counter,obj_fp);
    fclose(src_fp);
    fclose(obj_fp);
}
int main(int argc,char ** argv)
{
    int ch;
    char * obj_file = NULL;
    char * src_file = NULL;
    for(int i = 1; i < argc; i++)
    {
        char * arg = argv[i];
        if(!strcmp("-o",arg) && i + 1 < argc)
        {
            obj_file = malloc(strlen(argv[++i]) + 1);
            strcpy(obj_file,argv[i]);
        }
        else if(!strcmp("--help",arg))
        {
            usage();
            return 0;
        }
        else
        {
            src_file = malloc(strlen(argv[i]) + 1);
            strcpy(src_file,argv[i]);
        }
    }
    if(obj_file == NULL)
    {
        printf("Must input object file name\n");
        return -1;
    }
    if(src_file == NULL)
    {
        printf("Must input source file name\n");
        return -1;
    }
    return masm_main_loop(obj_file,src_file);
}

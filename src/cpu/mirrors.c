/*************************************************************************
	> File Name: mirriros.c
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Oct 2014 10:58:39 PM CST
 ************************************************************************/

#include "includes.h"
#define CPUID0 0x1234ABCD
#define CPUID1 0x11111111
int gsleep = 50 * 1000;
int htoi(const char *s)
{
    if( !s  )return 0;

    if( *s == '0'  )
    {
        s++;
        if( *s == 'x' || *s == 'X'  )s++;
    }
    int n = 0;
    while( *s  )
    {
                n <<= 4;
                if( *s <= '9'  )
                    n |= ( *s & 0xf  );
                else
                    n |= ( (*s & 0xf) + 9  );
                s++;
    }
    return n;
}
void loader(memory_t * m,char * bin_file,uint32_t offset)
{
    FILE * fp;
    fp = fopen(bin_file,"r");
    if(fp == NULL)
    {
        printf("Can not open %s:%s\n",bin_file,strerror(errno));
        exit(-1);
    }
    fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    uint32_t size = ftell(fp);
    uint32_t * buf = malloc(size);
    fseek(fp,0,SEEK_SET);
    fread(buf,size,1,fp);
    memcpy(m->memory + offset, buf,size);
}
int main(int argc,char ** argv)
{
    int32_t offset = 0x7c00;
    char * bin_file = NULL;
    for(int i = 1; i < argc; i++)
    {
        char * arg = argv[i];
        if(!strcmp(arg,"-o") && i + 1 < argc)
        {
            bin_file = argv[++i];
        }
        else if(!strcmp(arg,"--offset") && i + 1 < argc)
        {
            offset = htoi(argv[++i]);
        }
        else if(!strcmp(arg,"--interval") && i + 1 < argc)
        {
            gsleep = atoi(argv[++i]);
        }
        else
        {
            printf("Usage:      -o          : exe file path\n");
            printf("            --offset    : memory offset\n");
            exit(0);
        }
    }
    pthread_t runcpu, user_console, reg_console, mem_console;
    srand((unsigned int)time(NULL));
    memory_t * im_memory = init_memory(64);
    memory_t * re_memory = init_memory(64);
    cpu_t * imaginary   = init_cpu(CPUID0,offset,im_memory);
    cpu_t * real        = init_cpu(CPUID1,offset,re_memory);
    mirror_t mirror;
    mirror.imaginary = imaginary;
    mirror.real      = real;
    pthread_create(&runcpu,       NULL,cpu_main_loop,&mirror);

    pthread_mutex_t lock;
    pthread_mutex_init(&lock,NULL);
#if 1
    window_t * window = init_screen(&mirror,&lock);
    pthread_create(&mem_console,  NULL,mem_console_main_loop,&window[0]);
    pthread_create(&reg_console,  NULL,reg_console_main_loop,&window[1]);
    pthread_create(&user_console, NULL,user_console_main_loop,&window[2]);
#endif
    pthread_exit(NULL);
    endwin();
}

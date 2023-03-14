#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    char buf[128];
    unsigned long buf_adr = (unsigned long)&buf;  //###symbol
    //printf("buf adr : %lx \n", buf_adr);

    //getcwd
    //  unsigned long t0 = rdtsc(); 
    asm volatile("movq $79, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $128, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(buf_adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of getcwd: %d. \n", ret);
    //printf ("\nret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

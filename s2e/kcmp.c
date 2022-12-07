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
    int pid1;
    int pid2 = 1;
    int type = 2; //KCMP_FILES ##symbol

    //getpid
    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $39, %%rax; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(pid1)::"%rax");
    
    asm volatile("movq $312, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pid1),"m"(pid2),"m"(type):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of kcmp: %d. \n", ret);
    //printf ("ret of kcmp: %d. cy: %lu \n", ret, t1-t0);

    return 1;
}

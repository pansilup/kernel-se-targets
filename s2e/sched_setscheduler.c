#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    int pid = 0;
    int policy = 3; //SCHED_BATCH ##symbol
    struct sched_param param;
    param.sched_priority = 0;
    unsigned long adr_param = (unsigned long)&param;
    
    //unsigned long t0 = rdtsc();
    asm volatile("movq $144, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pid),"m"(policy),"m"(adr_param):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of sched_setscheduler: %d \n", ret);
    //printf ("ret of sched_setscheduler: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

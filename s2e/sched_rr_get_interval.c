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
    int pid = 0; //##symbol
    struct timespec tp;
    unsigned long tp_adr = (unsigned long)&tp;

    //unsigned long t0 = rdtsc();
    asm volatile("movq $148, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pid),"m"(tp_adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of sched_rr_get_interval: %d \n", ret);
    //printf ("ret of sched_rr_get_interval: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

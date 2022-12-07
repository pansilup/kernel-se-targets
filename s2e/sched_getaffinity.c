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
    unsigned long mask[64]; //this should be a ulong array
    unsigned long adr = (unsigned long)&mask;
    unsigned long size = 64;

	//unsigned long t0 = rdtsc(); 
    asm volatile("movq $204, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pid),"m"(size),"m"(adr):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of sched_getaffinity: %d, mask[0] %lu. \n", ret, mask[0]);
    //printf ("ret of sched_getaffinity: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

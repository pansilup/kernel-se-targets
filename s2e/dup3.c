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
    int flags = 524288; //##symbol

    //    unsigned long t0 = rdtsc();
    asm volatile("movq $292, %%rax; \n\t"
            "movq $2, %%rdi; \n\t"
            "movq $5, %%rsi; \n\t"	    
            "movq %1, %%rdx; \n\t"	    
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(flags):"%rax","%rdi","%rdx");
    //  unsigned long t1 = rdtsc();
  
    //printf ("ret of dup2: %d \n", ret);
    //  printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

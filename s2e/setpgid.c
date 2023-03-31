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
    int pgid = 0; //##symbol

    //  unsigned long t0 = rdtsc(); 
    asm volatile("movq $109, %%rax; \n\t"
            "movq $0, %%rdi; \n\t"
            "movq %0, %%rsi; \n\t"  //##symbol
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pgid):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of setpgid: %d. \n", ret);
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

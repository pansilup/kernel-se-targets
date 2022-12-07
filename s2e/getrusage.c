#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/resource.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    struct rusage usage;
    unsigned long adr = (unsigned long)&usage;
    unsigned long who = 0; //RUSAGE_SELF ##symbol

    //unsigned long t0 = rdtsc();
    asm volatile("movq $98, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(who),"m"(adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of getrusage: %d \n", ret);
    //printf ("ret : %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <errno.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    unsigned long ret;
    int arg = 15; //PR_SET_NAME ##symbol
    char buf[] = "krover-target"; 
    unsigned long adr = (unsigned long)&buf; //arg2;
    
    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $157, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq $0, %%rdx; \n\t" 
            "movq $0, %%r10; \n\t"  
            "movq $0, %%r8; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(arg),"m"(adr):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of prctl: %d\n", (int)ret);
    
    return 1;
}

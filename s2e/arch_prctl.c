#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm/prctl.h>
#include <sys/syscall.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    int code = 0x1004; //ARCH_GET_GS  ##symbol
    unsigned long addr = (unsigned long)&gs; //0;

    //unsigned long t0 = rdtsc();
    asm volatile("movq $158, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(code),"m"(addr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of arch_prctl: %d \n", ret);
    //printf ("ret of arch_prctl: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

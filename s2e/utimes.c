#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    char file[] = "/proc/cpuinfo";
    unsigned long filenm_adr = (unsigned long)&file;
    unsigned long times_adr = 0x0; // ##symbol
    
    //unsigned long t0 = rdtsc();
    asm volatile("movq $235, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(filenm_adr),"m"(times_adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of utimes: %d \n", ret);
    //printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

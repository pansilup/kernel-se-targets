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
    int list[32];
    unsigned long adr = (unsigned long)&list;
    int size = 32; //##symbol

    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $115, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(size),"m"(adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of getgroups: %d. \n", ret);
    //printf ("ret of getgroups: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

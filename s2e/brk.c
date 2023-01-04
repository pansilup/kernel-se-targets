#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <malloc.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    unsigned long ret;
    unsigned long adr;
    unsigned long new_brk = 0x0; //##symbol
     
    //unsigned long t0 = rdtsc();    
    asm volatile("movq $12, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(new_brk):"%rax","%rdi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of brk: %lx\n", ret);
    //printf ("prev brk %lx ret of brk: %lx\n", adr, ret);
    //printf ("ret of brk: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

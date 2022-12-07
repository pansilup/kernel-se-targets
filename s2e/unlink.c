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
    char fname[] = "old";
    unsigned long fname_adr = (unsigned long)&fname;

    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $87, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fname_adr):"%rax","%rdi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of unlink: %d. \n", ret);
    //printf ("\nret of unlink: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

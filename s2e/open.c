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
    char file_name[] = "/proc/cpuinfo";
    unsigned long name_adr = (unsigned long)&file_name;
    int flags = O_RDONLY; 
    int mode = 777; //##symbol 

    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $2, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $2, %%rsi; \n\t"
            "movq $3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(name_adr),"m"(flags),"m"(mode):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of open: %d \n", ret);
    //printf ("\nret of open: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

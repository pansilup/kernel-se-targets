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
    char dir[] = "env/dir"; //make sure that this directory already exists, we are testing a scall fail scenario
    unsigned long dir_adr = (unsigned long)&dir;

    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $83, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $0777, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(dir_adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of mkdir: %d. \n", ret);
    //printf ("\nret of mkdir: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

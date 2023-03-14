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
    int flags = 2048; 
    int pipefd[2] = {2,1}; 
    unsigned long bufadr = (unsigned long)&pipefd; //symbol
    //printf("bufadr %lx\n", bufadr);

    //unsigned long t0 = rdtsc();
    asm volatile("movq $293, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(bufadr),"m"(flags):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of pipe2: %d \n", ret);
    //printf ("ret of pipe2: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

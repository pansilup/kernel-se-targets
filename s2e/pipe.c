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
    int pipefd[2] = {2,1}; //### the content in this buffer are symbols
    unsigned long bufadr = (unsigned long)&pipefd;
    //printf("bufadr : %lx\n", bufadr);

    asm volatile("movq $22, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(bufadr):"%rax","%rdi");
  
    //printf ("ret of pipe: %d \n", ret);
  //  printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

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
    int seconds = 100; //### symbol
    //alarm
    asm volatile("movq $37, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"  //seconds ###stmbol is 100
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(seconds):"%rax","%rdi");
  
    //printf ("ret of alarm: %d \n", ret);
  //  printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

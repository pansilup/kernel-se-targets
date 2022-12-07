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
    char fname[] = "test.txt";
    unsigned long filename = (unsigned long)&fname;
    int mode = R_OK; //4 ##symbol
    
    //unsigned long t0 = rdtsc();
    asm volatile("movq $21, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(filename),"m"(mode):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of access: %d \n", ret);
    //printf ("ret of access: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

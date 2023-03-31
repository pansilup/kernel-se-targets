#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/resource.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    struct rlimit lim;
    lim.rlim_cur = 0;
    lim.rlim_max = 0;    //#symbol
    unsigned long adr = (unsigned long)&lim;
    
    //setrlimit
    asm volatile("movq $160, %%rax; \n\t"
            "movq $4, %%rdi; \n\t" //RLIMIT_CORE: core file sz
	    "movq %1, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(adr):"%rax","%rdi");
  
   //printf ("ret of setrlimit: %d \n", ret);
  //  printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

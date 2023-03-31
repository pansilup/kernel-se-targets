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
    struct rlimit lim_new;
    lim_new.rlim_cur = 0;
    lim_new.rlim_max = 0;    //#symbol
    unsigned long adr_new = (unsigned long)&lim_new;
    struct rlimit lim_old;
    unsigned long adr_old = 0x0;
    int ppid;

    asm volatile("movq $110, %%rax; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ppid)::"%rax","%rdi");
    //printf("ppid:%d\n", ppid);
    
    //prlimit64
    //unsigned long t0 = rdtsc();
    asm volatile("movq $302, %%rax; \n\t"
            "movq %1, %%rdi; \n\t" 
            "movq $0x4, %%rsi; \n\t" //RLIMIT_CORE: core file sz
	    "movq %2, %%rdx; \n\t"
	    "movq %3, %%r10; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(ppid),"m"(adr_new),"m"(adr_old):"%rax","%rdi");
    //unsigned long t1 = rdtsc();
  
    //printf ("ret of prlimit64: %d \n", ret);
    //printf ("ret of prlimit64: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

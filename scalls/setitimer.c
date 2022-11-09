#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    struct itimerval new_tstr;
    unsigned long new_tstr_adr = (unsigned long)&new_tstr;
    struct itimerval old_tstr;
    unsigned long old_tstr_adr = (unsigned long)&old_tstr;
    int which = ITIMER_REAL; //ITIMER_REAL 0, ITIMER_VIRTUAL 1, ITIMER_PROF 2 ##symbol
    
    new_tstr.it_interval.tv_sec = 0;
    new_tstr.it_interval.tv_usec = 0;
    new_tstr.it_value.tv_sec = 10;
    new_tstr.it_value.tv_usec = 0;
    
    asm volatile (
            "movq $0xabababababababab, %%rax; \n\t"
            "vmcall; \n\t"
            :::"%rax");

    sleep(0x5);

    // This is to issue an onsite analysis request
    asm volatile("movq $0xcdcdcdcd, %%rax; \n\t"
            "leaq 0x5(%%rip), %%rdi; \n\t"
            "movq $2, %%rdi; \n\t"
            "cmp $1, %%rdi; \n\t"
            "vmcall; \n\t"
            :::"%rax", "%rdi");

    //getpriority
//    unsigned long t0 = rdtsc();
    asm volatile("movq $38, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %2, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(which),"m"(new_tstr_adr),"m"(old_tstr_adr):"%rax","%rdi","%rsi");
  //  unsigned long t1 = rdtsc();
  
    printf ("ret of setitimer: %d \n", ret);
  //  printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

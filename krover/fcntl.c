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

    //fcntl
//    unsigned long t0 = rdtsc();
    asm volatile("movq $72, %%rax; \n\t"
            "movq $2, %%rdi; \n\t"
            "movq $0, %%rsi; \n\t" //F_DUPFD	    
            "movq $5, %%rdx; \n\t"	    
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret)::"%rax","%rdi","%rsi","%rdx");
  //  unsigned long t1 = rdtsc();
  
    printf ("ret of fcntl: %d \n", ret);
  //  printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    return 1;
}

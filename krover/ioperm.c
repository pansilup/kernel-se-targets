#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/io.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    unsigned long from = 0x378; //port 
    unsigned long num  = 32;    //num
    int           on   = 0;     //turn_on ##symbol
    
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

	    //  unsigned long t0 = rdtsc(); 
    asm volatile("movq $173, %%rax; \n\t"
            "movq $1, %%rdi; \n\t"
            "movq $2, %%rsi; \n\t"
            "movq $3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(from),"m"(num),"m"(on):"%rax","%rdi","%rsi","%rdx");
  //unsigned long t1 = rdtsc();
    
    printf ("ret of ioperm: %d. \n", ret);
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

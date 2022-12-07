#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;

    struct robust_list_head head;
    unsigned long head_adr = (unsigned long)&head;
    unsigned long len = 24; //sizeof(struct robust_list_head); ##symbol
    
	//unsigned long t0 = rdtsc(); 
    asm volatile("movq $273, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(head_adr),"m"(len):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of set_roboust_list: %d. \n", ret);
    //printf ("ret of set_roboust_list: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

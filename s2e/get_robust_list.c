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
    int pid = 0; //#symbol
    struct robust_list_head *head;
    unsigned long head_adr = (unsigned long)&head;
    unsigned long len = 24; //sizeof(struct robust_list_head);
    unsigned long lenptr = (unsigned long)&len;
    unsigned long len2 = 24;

	//unsigned long t0 = rdtsc(); 
    asm volatile("movq $273, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(head_adr),"m"(len2):"%rax","%rdi","%rsi");
    
    asm volatile("movq $274, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pid),"m"(head_adr),"m"(lenptr):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of get_roboust_list: %d. \n", ret);
    //printf ("ret of set_roboust_list: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

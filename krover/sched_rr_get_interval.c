#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    int pid = 0; //##symbol
    struct timespec tp;
    unsigned long tp_adr = (unsigned long)&tp;

    //for sched_setscheduler
    /*int ret2;
    int ppid = 0;
    int policy = 2; //SCHED_RR
    struct sched_param param;
    param.sched_priority = 77;
    unsigned long adr_param = (unsigned long)&param;*/

    
    asm volatile (
            "movq $0xabababababababab, %%rax; \n\t"
            "vmcall; \n\t"
            :::"%rax");

    sleep(0x5);

    //This is to issue an onsite analysis request
    asm volatile("movq $0xcdcdcdcd, %%rax; \n\t"
            "leaq 0x5(%%rip), %%rdi; \n\t"
            "movq $2, %%rdi; \n\t"
            "cmp $1, %%rdi; \n\t"
            "vmcall; \n\t"
            :::"%rax", "%rdi");

    //sched_Setscheduler policy to SCHED_RR
    /*asm volatile("movq $144, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret2):"m"(ppid),"m"(policy),"m"(adr_param):"%rax","%rdi","%rsi","%rdx");*/

    //unsigned long t0 = rdtsc();
    asm volatile("movq $148, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(pid),"m"(tp_adr):"%rax","%rdi","%rsi");
    //unsigned long t1 = rdtsc();
  
    printf ("ret of sched_rr_get_interval: %d \n", ret);
    //printf ("ret of getpriority: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

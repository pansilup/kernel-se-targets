#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    unsigned long ret_adr;
    unsigned long key = IPC_PRIVATE;
    unsigned long size = 4096;
    unsigned long flag = IPC_CREAT;
    unsigned long shmadr = 0x0;  //##symbol
    unsigned long shmflags = SHM_RDONLY; //0x1000

    //  unsigned long t0 = rdtsc(); 
    asm volatile("movq $29, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(key),"m"(size),"m"(flag):"%rax","%rdi","%rsi","%rdx");

    asm volatile("movq $30, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret_adr):"m"(ret),"m"(shmadr),"m"(shmflags):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of shmat: %lx. \n", ret_adr);
    //printf ("ret of shmat: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

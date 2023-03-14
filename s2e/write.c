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
    int pipefd[2] = {2,1};
    unsigned long bufadr = (unsigned long)&pipefd;  //###symbol
    char buf[32] = "abcdefg";
    unsigned long buf_adr = (unsigned long)&buf;
    //printf("buf_adr : %lx\n", buf_adr);

    //unsigned long t0 = rdtsc();
    asm volatile("movq $22, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(bufadr):"%rax","%rdi");
   
    int fd = pipefd[1];
    
    asm volatile("movq $1, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq $2, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fd),"m"(buf_adr):"%rax","%rdi","%rsi","%rdx");
      
    //unsigned long t1 = rdtsc();

    //printf ("ret of write: %d \n", ret);
    //printf ("ret of write: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

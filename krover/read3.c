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
    int ret1, ret2;
    int pipefd[2] = {2,1};
    unsigned long bufadr = (unsigned long)&pipefd;
    char buf[8] = "abcdefg";
    unsigned long buf_adr = (unsigned long)&buf;
    printf("buf_adr src data: %lx\n", buf_adr);
    char buf2[8] = {0x0};
    unsigned long buf_adr2 = (unsigned long)&buf2;
    printf("buf_adr read on to : %lx\n", buf_adr2);

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

    //unsigned long t0 = rdtsc();
    asm volatile("movq $22, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret1):"m"(bufadr):"%rax","%rdi");
   
    int fd_in = pipefd[1];
    int fd_out = pipefd[0];
    asm volatile("movq $1, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq $2, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret1):"m"(fd_in),"m"(buf_adr):"%rax","%rdi","%rsi","%rdx");
      
    asm volatile("movq $0, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq $2, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret2):"m"(fd_out),"m"(buf_adr2):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();

    printf ("ret1 : %d ret of read: %d %s \n", ret1, ret2, buf2);
    //printf ("ret of read: %d  cy : %lu\n", ret, t1-t0);
    
    return 1;
}

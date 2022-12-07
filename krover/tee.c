#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    unsigned long ret;
    int pipe1_fd[2];
    int pipe2_fd[2];
    unsigned long fdadr1 = (unsigned long)&pipe1_fd;
    unsigned long fdadr2 = (unsigned long)&pipe2_fd;
    int len = 2;
    unsigned int flags = 0x04; //SPLICE_F_MORE	##symbol
    char buf[] = "aa";
    unsigned long bufadr = (unsigned long)&buf;

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
    
    //unsigned long t1 = rdtsc();
    //creating first pipe
    asm volatile("movq $22, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fdadr1):"%rax","%rdi");
    
    //creating second pipe
    asm volatile("movq $22, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fdadr2):"%rax","%rdi");
    
    int fd_in = pipe1_fd[0];    //in syscall tee, copy from this fd(pipe1 read end)
    int fd_out = pipe2_fd[1];   //in tee, copy to this fd(pipe2 write end)
    int write_fd = pipe1_fd[1]; 
    
    //writing in to first pipe
    asm volatile("movq $1, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq $2, %%rdx; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(write_fd),"m"(bufadr):"%rax","%rdi","%rsi","%rdx");
    
    asm volatile("movq $276, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t" 
            "movq %4, %%r10; \n\t"  
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fd_in),"m"(fd_out),"m"(len),"m"(flags):"%rax","%rdi","%rsi","%rdx","%r10");
    //unsigned long t1 = rdtsc();
    
    printf ("ret of tee: %lx\n", ret);
    //printf ("ret of tee: %d. cpu-cycles: %lu \n", ret, t1-t0);
    
    return 1;
}

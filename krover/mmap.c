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
    int fd = -1;
    int prot = PROT_READ | PROT_WRITE; //0x3
    int flags = 0x2 | 0x20; //MAP_PRIVATE | MAP_ANONYMOUS; //PRIV:0x2,ANNO:0x20
    
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

    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $9, %%rax; \n\t"
            "movq $0x0, %%rdi; \n\t"
            "movq $1024, %%rsi; \n\t"
            "movq %1, %%rdx; \n\t" 
            "movq %2, %%r10; \n\t"  
            "movq %3, %%r8; \n\t" 
            "movq $0, %%r9; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(prot),"m"(flags),"m"(fd):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
    //unsigned long t1 = rdtsc();
    
    printf ("ret of mmap: %lx\n", ret);
    //printf ("ret of mmap: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

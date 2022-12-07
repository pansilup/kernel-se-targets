
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    unsigned long ret, ret2;
    int fd = -1;
    int prot = PROT_READ | PROT_WRITE; //0x3
    int flags2 = 0x2 | 0x20; //MAP_PRIVATE | MAP_ANONYMOUS; //PRIV:0x2,ANNO:0x20
    int flags = 0;
    
    void *maprw, *maprx;
    unsigned long page = 4096;
    maprw = mmap(NULL, page,
                 PROT_WRITE|PROT_READ,
                 MAP_ANON, -1, 0);

    maprx = mremap(maprw, page, NULL, page,1); // MAP_MAYMOVE);
    
    printf(" %lx %lx \n", (unsigned long)maprw, (unsigned long)maprx);
    return 0;

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
            "movq $16384, %%rsi; \n\t"
            "movq %1, %%rdx; \n\t" 
            "movq %2, %%r10; \n\t"  
            "movq %3, %%r8; \n\t" 
            "movq $0, %%r9; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret2):"m"(prot),"m"(flags2),"m"(fd):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
 
    ret2 = ret2 + (ret2 % 4096);

    //unsigned long t1 = rdtsc();
    asm volatile("movq $25, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $16384, %%rsi; \n\t"
            "movq $2048, %%rdx; \n\t" 
            "movq %2, %%r10; \n\t"  
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(ret2),"m"(flags):"%rax","%rdi","%rsi","%rdx","%r10");
    

    printf ("ret of mmap: %lx mremap: %lx\n", ret2, ret);
    //printf ("ret of mremap: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

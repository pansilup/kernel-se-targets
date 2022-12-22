#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <malloc.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    unsigned long ret;
    unsigned long ret2;
    int prot2 = PROT_READ | PROT_WRITE; //0x3 
    int flags = MAP_PRIVATE | MAP_ANONYMOUS; //0x16
    unsigned long prot = 0x0000000000000001; //PROT_READ 0x1 ##symbol  
    unsigned long sizes = 4096*4;
    unsigned long len = 4096;

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
    asm volatile("movq $9, %%rax; \n\t"
            "movq $0x0, %%rdi; \n\t"
            "movq %3, %%rsi; \n\t"
            "movq %1, %%rdx; \n\t" //PROT_READ | WRITE
            "movq %2, %%r10; \n\t" //MAP_PRIVATE | ANONYMOUS. 
            "movq $-1, %%r8; \n\t" 
            "movq $0, %%r9; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret2):"m"(prot2),"m"(flags),"m"(sizes):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
    unsigned long adr = ret2 + (ret2 % 4096);
  
    asm volatile("movq $10, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t" //PROT_READ | WRITE
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(adr),"m"(len),"m"(prot):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
    
    printf ("ret of mmap %lx ret of mprotect: %lx\n", adr, ret);
    //printf ("ret of mprotect: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

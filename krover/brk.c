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
    unsigned long adr;

    /*asm volatile("movq $12, %%rax; \n\t"
            "movq $0, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(adr)::"%rax","%rdi");
    unsigned long new_brk = adr + 4096;
    printf("prev brk %lx new brk %lx\n", adr, new_brk);
    */    
    unsigned long new_brk = 0x0; //##symbol
    
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
    asm volatile("movq $12, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(new_brk):"%rax","%rdi");
    //unsigned long t1 = rdtsc();
    
    printf ("ret of brk: %lx\n", ret);
    //printf ("prev brk %lx ret of brk: %lx\n", adr, ret);
    //printf ("ret of brk: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

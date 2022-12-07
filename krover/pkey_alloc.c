#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/mman.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret;
    unsigned int access_rights = PKEY_DISABLE_ACCESS; //0x1 ##symbol
    
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

//  unsigned long t0 = rdtsc(); 
    asm volatile("movq $330, %%rax; \n\t"
            "movq $0x0, %%rdi; \n\t"
            "movq $0x0, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret)::"%rax","%rdi","%rsi");
  //unsigned long t1 = rdtsc();
    
    printf ("ret of pkey_alloc: %d. \n", ret);
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

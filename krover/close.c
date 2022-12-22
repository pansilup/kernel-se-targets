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
    int ret, ret1, ret2;
    char file_name[] = "/proc/cpuinfo";
    unsigned long name_adr = (unsigned long)&file_name;
    int flags = O_RDONLY; //flags ##symbol
    int mode = 777; //##mode 

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
    asm volatile("movq $2, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $2, %%rsi; \n\t"
            "movq $3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret1):"m"(name_adr),"m"(flags),"m"(mode):"%rax","%rdi","%rsi");
    asm volatile("movq $2, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $2, %%rsi; \n\t"
            "movq $3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret2):"m"(name_adr),"m"(flags),"m"(mode):"%rax","%rdi","%rsi");
    
    asm volatile("movq $3, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(ret2):"%rax","%rdi");
    //unsigned long t1 = rdtsc();
    
    printf ("ret1:%d ret2:%d ret of close: %d \n", ret1, ret2, ret);
    //printf ("\nret of close: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

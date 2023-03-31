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
    int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
    int flags = 0x1; 

    char file_name[] = "env/old";
    unsigned long name_adr = (unsigned long)&file_name;
    int file_flags = O_RDWR; 
    int mode = S_IRWXO | S_IRUSR | S_IWUSR;  
    unsigned long adr_to_mp = 0x0; //##symbol
    unsigned long offset = 0;

    asm volatile("movq $2, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $2, %%rsi; \n\t"
            "movq $3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(fd):"m"(name_adr),"m"(file_flags),"m"(mode):"%rax","%rdi","%rsi");

    asm volatile("movq $9, %%rax; \n\t"
            "movq %4, %%rdi; \n\t"
            "movq $1024, %%rsi; \n\t"
            "movq %1, %%rdx; \n\t" 
            "movq %2, %%r10; \n\t"  
            "movq %3, %%r8; \n\t" 
            "movq %5, %%r9; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(prot),"m"(flags),"m"(fd),"m"(adr_to_mp),"m"(offset):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
    
    printf ("ret of open %d mmap: %lx\n", fd, ret);
    //printf ("ret of mmap: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

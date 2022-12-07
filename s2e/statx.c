#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/stat.h> //if issuing the scall through libc use sys/stat.h as per the man pages
//here we do not issue through libc

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    unsigned long ret;
    int dirfd = -100; // AT_FDCWD	
    char path[] = "env/file.txt";
    unsigned long path_adr = (unsigned long)&path;
    int flags = 0;
    unsigned int mask = 0x00000001U | 0x00000002U; //STATX_TYPE, STATX_MODE ##symbol
    struct statx statxbuf;
    unsigned long statxbuf_adr = (unsigned long)&statxbuf;
    
    //unsigned long t0 = rdtsc(); 
    asm volatile("movq $332, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t" 
            "movq %4, %%r10; \n\t"  
            "movq %5, %%r8; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(dirfd),"m"(path_adr),"m"(flags),"m"(mask),"m"(statxbuf_adr):"%rax","%rdi","%rsi","%rdx","%r10","%r8");
    //unsigned long t1 = rdtsc();
    
    //printf ("ret of statx: %lx\n", ret); 
    //printf ("ret of statx: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

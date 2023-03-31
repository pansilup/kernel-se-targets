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
    char file_name[] = "/proc/cpuinfo";
    unsigned long fname_adr = (unsigned long)&file_name;
    int fflags = __O_PATH;
    int fmode = 777; 

    unsigned long ret, fret;
    char path[] = "";
    unsigned long path_adr = (unsigned long)&path;
    int flags = 0x0; //#symbol  , to suceed flafs must be 0x1000
    unsigned int mask = 0x00000200U; //STATX_SIZE

    struct statx statxbuf;
    unsigned long statxbuf_adr = (unsigned long)&statxbuf;
    
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

    asm volatile("movq $2, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $2, %%rsi; \n\t"
            "movq $3, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(fret):"m"(fname_adr),"m"(fflags),"m"(fmode):"%rax","%rdi","%rsi");

//  unsigned long t0 = rdtsc(); 
    asm volatile("movq $332, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t" 
            "movq %4, %%r10; \n\t"  
            "movq %5, %%r8; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fret),"m"(path_adr),"m"(flags),"m"(mask),"m"(statxbuf_adr):"%rax","%rdi","%rsi","%rdx","%r10","%r8");
  //unsigned long t1 = rdtsc();
    
    printf ("ret of statx: %lx\n", ret); 
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    
    return 1;
}

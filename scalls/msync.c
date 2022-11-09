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
    unsigned long ret, adr;
    int 	fd; 
    char 	file[] = "test.txt";
    unsigned long fnameadr = (unsigned long)&file;
    int 	file_flags = O_RDWR; 
    int 	mode = 0644;

    int prot = PROT_READ | PROT_WRITE; //0x3 ##symbolic
    int flags = MAP_SHARED | MAP_POPULATE; //0x1
    
    int msync_flags = MS_ASYNC; //0x1 ##symbol
    
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
 asm volatile("movq $2, %%rax; \n\t" //open
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            //"movq %3, %%rdx; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(fd):"m"(fnameadr),"m"(file_flags):"%rax","%rdi","%rsi", "%rdx");

    asm volatile("movq $9, %%rax; \n\t"
            "movq $0x0, %%rdi; \n\t"
            "movq $1024, %%rsi; \n\t"
            "movq %1, %%rdx; \n\t" //PROT_READ | WRITE
            "movq %2, %%r10; \n\t" //MAP_PRIVATE | ANONYMOUS. 
            "movq %3, %%r8; \n\t" 
            "movq $0, %%r9; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(adr):"m"(prot),"m"(flags),"m"(fd):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
 
    asm volatile("movq $26, %%rax; \n\t" //open
            "movq %1, %%rdi; \n\t"
            "movq $1024, %%rsi; \n\t"
            "movq %2, %%rdx; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(adr),"m"(msync_flags):"%rax","%rdi","%rsi", "%rdx");
  //unsigned long t1 = rdtsc();
    
    printf ("fd: %d, adr: %lx, ret of msync :%lx \n", fd, adr, ret);
    
    //ret = mmap(NULL, 1024, PROT_READ, MAP_ANONYMOUS, -1, 0);
    //printf ("ret of mmap: %lx %d \n", ret, errno);
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

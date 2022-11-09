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
    void * m;
    unsigned long ret, ret1;
    int prot_wr = PROT_WRITE; 	//0x2 ##symbolic
    int prot_rd = PROT_READ; 	//0x1
    int flags = MAP_PRIVATE | MAP_ANONYMOUS; //0x16

  //  sysconf(_SC_PAGE_SIZE);
//    ret = mprotect(ret1, 4096, PROT_WRITE);

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
/*    
    asm volatile("movq $9, %%rax; \n\t"
            "movq $0x0, %%rdi; \n\t"
            "movq $4096, %%rsi; \n\t"
            "movq %1, %%rdx; \n\t" //PROT_READ
            "movq %2, %%r10; \n\t" //MAP_PRIVATE | ANONYMOUS. 
            "movq $-1, %%r8; \n\t" 
            "movq $0, %%r9; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(prot_rd),"m"(flags):"%rax","%rdi","%rsi","%rdx","%r10","%r8","%r9");
*/
    m = memalign(4096, 4*4096);
    ret1 = (unsigned long)m;
    ret1 += (2*4096);
    asm volatile("movq $10, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $4096, %%rsi; \n\t"
            "movq $0x1, %%rdx; \n\t" //PROT_WRITE
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(ret1):"%rax","%rdi","%rsi","%rdx");
    //unsigned long t1 = rdtsc();
    
    printf ("ret1 %lx ret of mprotect: %lx \n", ret1, ret);
    
    //ret = mmap(NULL, 1024, PROT_READ, MAP_ANONYMOUS, -1, 0);
    //printf ("ret of mmap: %lx %d \n", ret, errno);
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}


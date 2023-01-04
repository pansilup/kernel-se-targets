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
    int prot = PROT_READ | PROT_WRITE | PROT_EXEC; //0x3
    int flags = 0x1; //0x2 | 0x20; //MAP_PRIVATE | MAP_ANONYMOUS; //PRIV:0x2,ANNO:0x20

    char file_name[] = "env/old";
    unsigned long name_adr = (unsigned long)&file_name;
    int file_flags = O_RDWR; //O_APPEND; //flags ##symbol
    int mode = S_IRWXO | S_IRUSR | S_IWUSR; //777; //##mode 
    unsigned long adr_to_mp = 0x0;
    unsigned long offset = 0;

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
    //unsigned long t1 = rdtsc();
    
    printf ("ret of open %d mmap: %lx\n", fd, ret);
    //printf ("ret of mmap: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

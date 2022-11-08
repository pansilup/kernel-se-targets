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
    int ret;
    int fd = open("test.txt", O_WRONLY | O_CREAT, 0644);
    printf ("file fd: %d\n", fd);

    asm volatile (
            "movq $0xabababababababab, %%rax; \n\t"
            "vmcall; \n\t"
            :::"%rax");

    sleep(0x5);

    /* This is to issue an onsite analysis request */
    asm volatile("movq $0xcdcdcdcd, %%rax; \n\t"
            "leaq 0x5(%%rip), %%rdi; \n\t"
            "movq $2, %%rdi; \n\t"
            "cmp $1, %%rdi; \n\t"
            "vmcall; \n\t"
            :::"%rax", "%rdi");
    
    //lseek
    
    asm volatile("movq $8, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $10, %%rsi; \n\t"
            "movq $1, %%rdx; \n\t"  //SEEK_CUR 1, SEEK_SET 0, SEEK_END 2
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fd):"%rax","%rdi","%rsi", "%rdx");
    printf ("\nlseek return: %d. \n", ret); 
    
    return 1;
}

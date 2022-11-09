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
    int 	ret;
    int 	fd; 
    char 	file[] = "test.txt";
    unsigned long fnameadr = (unsigned long)&file;
    int 	flags = O_WRONLY | O_CREAT; 
    int 	mode = 0644;
    int 	operation = 1; //LOCK_SH ##symbol

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
    
    asm volatile("movq $2, %%rax; \n\t" //open
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq %3, %%rdx; \n\t" 
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(fd):"m"(fnameadr),"m"(flags),"m"(mode):"%rax","%rdi","%rsi", "%rdx");
    
    asm volatile("movq $73, %%rax; \n\t" //lseek
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fd),"m"(operation):"%rax","%rdi","%rsi");
    
    printf ("\nfd %d flock return: %d. \n", fd, ret); 
    
    return 1;
}

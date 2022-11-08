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
    char fname[] = "old";
    unsigned long fname_adr = (unsigned long)&fname;
    //printf("dir_name_adr : %lx\n", dir_adr);
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
    asm volatile("movq $90, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $0777, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(fname_adr):"%rax","%rdi","%rsi");
  //unsigned long t1 = rdtsc();
    
    printf ("ret of mkdir: %d. \n", ret);
    //printf ("\nret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

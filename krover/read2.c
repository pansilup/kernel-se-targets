#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{
    int ret, ret1;
    char buf[32];
    unsigned long buf_adr = (unsigned long)&buf;
    char fname[] = "test.txt";
    unsigned long filename_adr = (unsigned long)&fname;
    //=open("test.txt", O_RDONLY);
    int flags = O_RDONLY;

    ret1 = open("test.txt", O_RDONLY);
    
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
    
    //ret1 = open("test.txt", O_RDONLY);
/*    asm volatile("movq $2, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            //"movq $10, %%rdx; \n\t" // n.a. since not O_CREAT | O_TMPFILE
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret1):"m"(filename_adr),"m"(flags):"%rax","%rdi","%rsi");
*/
   asm volatile("movq $0, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq %2, %%rsi; \n\t"
            "movq $10, %%rdx; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(ret1),"m"(buf_adr):"%rax","%rdi","%rsi","%rdx");
  //unsigned long t1 = rdtsc();
    //ret = read(fd, buf_adr, 10);
    printf ("ret1:%d\nret of read: %d \nerrno:%d \n", ret1, ret,errno);
    //printf ("\nret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

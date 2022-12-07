#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/capability.h>
#include <sys/syscall.h>

static __attribute__ ((noinline)) unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long long) lo | (unsigned long long) hi << 32);
}

int main (void)
{

    struct __user_cap_header_struct hdr;
    ulong adr = (ulong)&hdr;
    int ret;

    hdr.version = _LINUX_CAPABILITY_VERSION_3; //0x20080522 ##symbol
    hdr.pid = 0;

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
    asm volatile("movq $125, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"
            "movq $0x0, %%rsi; \n\t"
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(adr):"%rax","%rdi","%rsi");
  //unsigned long t1 = rdtsc();
    
    printf ("ret of capget: %d. \n", ret);
    //printf ("ret of setpriority: %d. cy: %lu \n", ret, t1-t0);
    return 1;
}

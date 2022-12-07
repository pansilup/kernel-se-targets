#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDR "172.217.160.99"
#define SERVER_PORT 80

int main()
{
    int ret;
    struct sockaddr my_addr;
    unsigned long adr = (unsigned long)&my_addr;
    int sockfd;
    socklen_t len = sizeof(my_addr);
    unsigned long len_adr = (unsigned long)&len;    
    memset(&my_addr, sizeof(my_addr), 0);
    //sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
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

    //socket
    
    asm volatile("movq $41, %%rax; \n\t"
            "movq $2, %%rdi; \n\t"    //AF_INET 2
            "movq $1, %%rsi; \n\t"   //SOCK_STREAM 1
            "movq $0, %%rdx; \n\t"    //IPPROTO_IP 0
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(sockfd)::"%rax","%rdi","%rsi", "%rdx");

    asm volatile("movq $51, %%rax; \n\t"
            "movq %1, %%rdi; \n\t"    //fd
            "movq %2, %%rsi; \n\t"    
            "movq %3, %%rdx; \n\t"    //IPPROTO_IP 0
            "syscall; \n\t"
            "movq %%rax, %0; \n\t"
            :"=m"(ret):"m"(sockfd),"m"(adr),"m"(len_adr):"%rax","%rdi","%rsi", "%rdx");
    
    //ret = getsockname(sockfd, adr, len_adr);
    printf ("sockfd : %d ret of getsockname: %d \n", sockfd, ret);    

    return 0;
}

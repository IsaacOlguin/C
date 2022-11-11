/* Export functions here */

#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <asm/unistd.h>

/*
static ssize_t(*real_write)(int fd, const void *buf, size_t count) = NULL;
static ssize_t(*real_read)(int fd, const void *buf, size_t count) = NULL;
__asm__(
            "movl %1, %%eax;"
            "movl %2, %%rdi;"
            "movl %3, %%rsi;"
            "movl %4, %%rdx;"
            : "=r" (tamanio)
            : "r" (__NR_write), "D" (fd), "S" (buf), "d" (count)
        );
    
*/

ssize_t write(int fd, const void *buf, size_t count) {
    if(count > 0) {
        /*
        ssize_t tamanio;       

        int longi = count;
        register int    syscall_no  asm("rax") = 1;
        register int    arg1        asm("rdi") = 1;
        register char*  arg2        asm("rsi") = buf;
        register size_t    arg3        asm("rdx") = longi;
        asm("syscall");

        return tamanio;        
        */
        ssize_t ret;
        asm volatile
        (
            "syscall"
            : "=a" (ret)
            //                 EDI      RSI       RDX
            : "0"(__NR_write), "D"(4), "S"(buf), "d"(count)
            : "rcx", "r11", "memory"
        );
        if(ret < 0 ) {
            errno = ret*-1;
            ret = -1;
        }
        return ret;
    }
}


ssize_t read(int fd, const void *buf, size_t count) {
    if(count > 0) {
        ssize_t ret;
        asm volatile
        (
            "syscall"
            : "=a" (ret)
            //                 EDI      RSI       RDX
            : "0"(__NR_read), "D"(fd), "S"(buf), "d"(count)
            : "rcx", "r11", "memory"
        );
        if(ret < 0 ) {
            errno = ret*-1;
            ret = -1;
        }
        return ret;       
    }
}
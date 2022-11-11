#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h> //pid
#include <ctype.h>

/* POSIX */
#include <unistd.h>
#include <sys/user.h>
#include <sys/wait.h>

/* Linux */
#include <syscall.h>
#include <sys/reg.h>
#include <sys/ptrace.h>

int main(int argc, char **argv) {
    long result;

    if(argc <= 1) {
        printf("Too few arguments <<%i>>\n", argc);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    switch (pid) {
        case -1: // Error
            return -1;
            break;
            
        case 0:  // Child
            ptrace(PTRACE_TRACEME, 0, 0, 0);
            // execvp will block until the parent attaches and allows to continue
            execvp(argv[1], argv + 1);
            break;
    }

    waitpid(pid, 0, 0); // Sync with execvp
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL);

    while(1) {
        // System call
        result = ptrace(PTRACE_SYSCALL, pid, 0, 0);
        if (result == -1) {
            printf("[1] %s\n", strerror(errno));
            return 0;
        }

        result = waitpid(pid, 0, 0);
        if (result == -1) {
            printf("[2] %s\n", strerror(errno));
        }

        // Get system call arguments 
        struct user_regs_struct regs;
        if (ptrace(PTRACE_GETREGS, pid, 0, &regs) == -1) {
            printf("[3] %s\n", strerror(errno));
        }

        // Handling per system call 
        switch (regs.orig_rax) {
            case SYS_write:
                //printf("Write was invoked\n");
                fprintf(stderr, "%s(%ld, %p, %ld) = %ld\n", "write", (long)regs.rdi, regs.rsi, (long)regs.rdx, (long)regs.rdx);

                break;
            case SYS_read:
                //printf("Read was invoked\n");

                fprintf(stderr, "%s(%ld, %p, %ld) = %ld\n", "read", (long)regs.rdi, regs.rsi, (long)regs.rdx, (long)regs.rdx);
                break;
            
            case SYS_exit:
                //printf("Exit was invoked\n");
                return 0;

            case SYS_exit_group:
                //printf("Exit_group was invoked\n");
                return 0;
            
            /*
            default:
                printf("Instruction [%i]\n", regs.orig_rax);
            */
        }
    }

    return 0;
}
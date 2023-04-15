#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/reg.h>

void child()
{
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    execl("/bin/echo", "/bin/echo", "Hello, world!", NULL);
}

void parent(pid_t pid)
{
    int status;
    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

    while (!WIFEXITED(status))
    {
        struct user_regs_struct state;
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)
        {
            ptrace(PTRACE_GETREGS, pid, 0, &state);
            printf("SYSCALL %d at %08lx\n", state.orig_rax, state.rip);
            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            waitpid(pid, &status, 0);
        }
    }
}

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid)
        parent(pid);
    else
        child();
    return 0;
}

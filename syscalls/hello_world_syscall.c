#include <unistd.h>
#include <sys/syscall.h>

ssize_t my_write(int fd, const void *buf, size_t count) 
{
        return syscall(SYS_write, fd, buf, count);
}

int main() 
{
        const char *message = "Hello!!!\n";
        my_write(STDOUT_FILENO, message, 9);
        return 0;
}

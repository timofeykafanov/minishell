#include <unistd.h>
int main(void)
{
    int fd[2];

    pipe(fd);

    int pid1 = fork();
    
    return (0);
}
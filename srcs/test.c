#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int ac, char **av, char **env)
{
    int fd1[2];
    int fd2[2];

    pipe(fd1);
	// test
    char *argv1[] = {"cat", "allowed_funcs.txt", NULL};
    char *argv2[] = {"grep", "si", NULL};
    char *argv3[] = {"sort", NULL};
	char *argv4[] = {"wc", "-l", NULL};

    int pid1 = fork();

    if (pid1 == 0)
    {
        dup2(fd1[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        // close(fd2[0]);
        // close(fd2[1]);
        execve("/usr/bin/cat", argv1, env);
    }

    pipe(fd2);

    int pid2 = fork();

    if (pid2 == 0)
    {
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        execve("/usr/bin/grep", argv2, env);
    }

	close(fd1[0]);
	close(fd1[1]);
	fd1[0] = fd2[0];
	fd1[1] = fd2[1];

	pipe(fd2);

    int pid3 = fork();

    if (pid3 == 0)
    {
        dup2(fd1[0], 0);
		dup2(fd2[1], 1);
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        execve("/usr/bin/sort", argv3, env);
    }

	close(fd1[0]);
	close(fd1[1]);
	fd1[0] = fd2[0];
	fd1[1] = fd2[1];
	
	int pid4 = fork();

	if (pid4 == 0)
	{
		dup2(fd1[0], 0);
		close(fd1[0]);
		close(fd1[1]);
		// close(fd2[0]);
		// close(fd2[1]);
		execve("/usr/bin/wc", argv4, env);
	}

    close(fd1[0]);
    close(fd1[1]);
    // close(fd2[0]);
    // close(fd2[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
	waitpid(pid4, NULL, 0);
    return (0);
}
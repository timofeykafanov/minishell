/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 12:37:17 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/26 17:10:54 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// char    *ft_execve(t_memory *memory, t_commands *cmd)
// {
//     pid_t	pid;
//     int		status;
//     char    **argv;
//     int     i;

//     i = 0;
//     while (cmd->args)
//     {
//         i++;
//         cmd->args = cmd->args->next;
//     }
//     argv = (char **)malloc(sizeof(char *) * (i + 1));
//     i = 0;
//     while (cmd->args)
//     {
//         argv[i] = strdup(cmd->args->name);
//         cmd->args = cmd->args->next;
//         i++;
//     }
//     argv[i] = NULL;

//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork failed");
//         return (NULL);
//     }
//     if (pid == 0)
//     {
//         if (execve(ft_strjoin("/usr/bin/", cmd->name), argv, memory->env) == -1)
//         {
//             perror("execve failed");
//             exit(EXIT_FAILURE);
//         }
//     }
//     else
//     {
//         waitpid(pid, &status, 0);
//     }
//     return ();
// }

char *ft_execve(t_memory *memory, t_commands *cmd, int input_fd, bool flag, int pipefd[2])
{
	pid_t   pid;
	int	 status;
	char	**argv;
	int	 i;
	// int	 pipefd[2];
	// char	buffer[1024];
	// ssize_t nbytes;
	char	*full_path;
	char	*output = NULL;
	// size_t  output_size = 0;

	i = 0;
	t_commands *arg = cmd->args;
	while (arg)
	{
		i++;
		arg = arg->next;
	}
	argv = (char **)malloc(sizeof(char *) * (i + 1));
	if (argv == NULL)
	{
		perror("malloc failed");
		return (NULL);
	}
	i = 0;
	arg = cmd->args;
	while (arg)
	{
		if (i == 0)
			argv[i] = ft_strdup(cmd->name);
		else
		{
			argv[i] = ft_strdup(arg->name);
			arg = arg->next;
		}
		if (argv[i] == NULL)
		{
			perror("strdup failed");
			return (NULL);
		}
		i++;
	}
	argv[i] = NULL;

	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (NULL);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (NULL);
	}

	(void)input_fd;
	(void)flag;
	full_path = ft_strjoin("/usr/bin/", cmd->name);
	if (full_path == NULL)
	{
		perror("ft_strjoin failed");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) { // Child process
	if (input_fd != -1) { // For the second command onwards
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (cmd->next) { // If there is a next command
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	execve(full_path, argv, memory->env);
	perror("execve failed");
	free(full_path);
	exit(EXIT_FAILURE);
	} else { // Parent process
		if (input_fd != -1) {
			close(input_fd);
		}
		if (cmd->next) {
			close(pipefd[1]); // Close the write end in the parent
			input_fd = pipefd[0]; // Set input for the next command
		}
		waitpid(pid, &status, 0);
	}
	// if (pid == 0)
	// {
	//	 if (input_fd != -1)
	//	 {
	//		 dup2(input_fd, STDIN_FILENO);
	//		 close(input_fd);
	//	 }
	//	 printf("cmd->next == NULL %d\n", cmd->next != NULL);
	//	 if (cmd->next != NULL)
	//	 {
	//		 printf("TEST\n");
	//		 dup2(pipefd[1], STDOUT_FILENO);
	//		 close(pipefd[0]);
	//		 close(pipefd[1]);
	//		 execve(full_path, argv, memory->env);
	//	 }
	//	 dup2(pipefd[0], STDIN_FILENO);
	//	 close(pipefd[0]);
	//	 close(pipefd[1]);
	//	 // printf("2 exec %s\n", full_path);
	//	 execve(full_path, argv, memory->env);
	//	 perror("execve failed");
	//	 free(full_path);
	//	 exit(EXIT_FAILURE);
	// }
	// else
	// {
	//	 // if (input_fd != -1)
	//	 //	 close(input_fd);
	//	 // if (cmd->next)
	//	 //	 close(pipefd[1]);

	//	 // if (flag)
	//	 // {
	//	 //	 while ((nbytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
	//	 //	 {
	//	 //		 buffer[nbytes] = '\0';
	//	 //		 output_size += nbytes;
	//	 //		 output = realloc(output, output_size + 1);
	//	 //		 if (output == NULL)
	//	 //		 {
	//	 //			 perror("realloc failed");
	//	 //			 return (NULL);
	//	 //		 }
	//	 //		 ft_strlcat(output, buffer, ft_strlen(output) + nbytes + 1);
	//	 //	 }
	//	 // }

	//	 // if (cmd->next)
	//	 //	 input_fd = pipefd[0];
	//	 // else
	//	 //	 close(pipefd[0]);
	//	 close(pipefd[0]);
	//	 close(pipefd[1]);
	//	 waitpid(pid, &status, 0);
	// }

	i = 0;
	while (argv[i] != NULL)
	{
		free(argv[i]);
		i++;
	}
	free(argv);

	return (output);
}
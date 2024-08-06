/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:44:21 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/06 15:35:33 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void execute_ls(void)
{
	DIR				*d;
	int				current_width;
	struct dirent	*dir;
	struct winsize	w;

	current_width = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			int name_length = strlen(dir->d_name) + 1;
			if (current_width + name_length > w.ws_col / 1.7)
			{
				printf("\n");
				current_width = 0;
			}
			if (*dir->d_name != '.')
			{
				printf("%s\t", dir->d_name);
				current_width += name_length;
			}
		}
		printf("\n");
		closedir(d);
	}
	else
		perror("opendir() error");
}

static void execute_command(char **args, t_memory *memory)
{
	if (ft_strncmp(args[0], CD, ft_strlen(args[0])) == 0)
	{
        if (args[1] != NULL)
            execute_cd(args[1], memory);
        else
            perror("cd: missing argument\n");
    }
	else if (ft_strncmp(args[0], PWD, ft_strlen(args[0])) == 0)
        execute_pwd(memory);
	else if (ft_strncmp(args[0], "ls", ft_strlen(args[0])) == 0)
        execute_ls();
	else if (ft_strncmp(args[0], EXIT, ft_strlen(args[0])) == 0)
	{
		free_memory(memory);
        exit(SUCCESS);
    }
	else if (ft_strncmp(args[0], ENV, ft_strlen(args[0])) == 0)
		print_env(memory);
	else if (ft_strncmp(args[0], EXPORT, ft_strlen(EXPORT)) == 0)
		print_export(memory);
	// else if (strcmp(args[0], "unset") == 0)
	// 	unset_env(memory);
	else
        printf("Unknown command: %s\n", args[0]);
}

static void handle_redirection(char **args) {
	int i = 0;
	// int fd_in = 0;
	// int fd_out = 1;
	int fd;

	while (args[i] != NULL) {
		if (strcmp(args[i], "<") == 0) {
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1) {
				perror("open() error");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
		} else if (strcmp(args[i], ">") == 0) {
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1) {
				perror("open() error");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
		}
		i++;
	}
}

static void execute_piped_commands(char **commands, int num_commands, t_memory *memory) {
    int pipefd[2];
    int fd_in = 0;

    for (int i = 0; i < num_commands; i++) {
        if (strcmp(commands[i], "|") == 0) {
            pipe(pipefd);

            if (fork() == 0) {
                dup2(fd_in, 0);
				if (i < num_commands - 1) {
                    dup2(pipefd[1], 1);
                }
                close(pipefd[0]);
                char *cmd_args[100];
                int cmd_index = 0;
                for (int j = 0; j < i; j++) {
                    cmd_args[cmd_index++] = commands[j];
                }
                cmd_args[cmd_index] = NULL;
                execute_command(cmd_args, memory);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
                close(pipefd[1]);
                fd_in = pipefd[0];
            }
        }
    }
}

void parse_and_execute_tokens(t_tokens *tokens, t_memory *memory) {
    t_tokens *current = tokens;
    char *args[100];
    int arg_index = 0;
    char *commands[100];
    int num_commands = 0;
    int in_pipe = 0;

    while (current != NULL) {
        if (strcmp(current->data, "|") == 0) {
            args[arg_index] = NULL;
            commands[num_commands++] = strdup(args[0]);
            for (int i = 1; i < arg_index; i++) {
                commands[num_commands++] = strdup(args[i]);
            }
            commands[num_commands++] = strdup("|");
            arg_index = 0;
            in_pipe = 1;
        } else if (strcmp(current->data, ">") == 0 || strcmp(current->data, "<") == 0) {
            args[arg_index] = NULL;
            handle_redirection(args);
            arg_index = 0;
        } else {
            args[arg_index++] = current->data;
        }
        current = current->next;
    }

    if (arg_index > 0) {
        args[arg_index] = NULL;
        if (in_pipe) {
            commands[num_commands++] = strdup(args[0]);
            for (int i = 1; i < arg_index; i++) {
                commands[num_commands++] = strdup(args[i]);
            }
            execute_piped_commands(commands, num_commands, memory);
        } else {
            execute_command(args, memory);
        }
    }
}

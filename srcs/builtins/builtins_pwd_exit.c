/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/20 17:18:51 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_pwd(t_memory *memory)
{
	getcwd(memory->pwd, PATH_MAX);
	if (memory->pwd != NULL)
		ft_printf("%s\n", STDOUT_FILENO, memory->pwd);
	else
		perror("pwd");
}

static bool	contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	exit_shell(t_memory *memory, bool is_redir, int saved_fds[2])
{
	if (is_redir && saved_fds)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	free_memory(memory);
	// ft_printf("exit\n", STDOUT_FILENO);
	exit(memory->exit_status);
}

void	execute_exit(t_memory *memory, bool is_redir, int saved_fds[2])
{
	t_command	*command;

	command = memory->commands;
	if (command->args[1] && contains_only_digits(command->args[1]) && command->args[2])
	{
		// ft_printf("exit\n", STDERR_FILENO);
		ft_printf("exit: too many arguments\n", STDERR_FILENO);
		memory->exit_status = 1;
		memory->exit_failed = true;
		return ;
	}
	else if (command->args[1])
	{
		if (contains_only_digits(command->args[1]) && command->args[1][0])
			memory->exit_status = ft_atoi(command->args[1]) % 256;
		else
		{
			// ft_printf("exit\n", STDERR_FILENO);
			ft_printf("exit: %s: numeric argument required\n", \
				STDERR_FILENO, command->args[1]);
			memory->exit_status = 2;
		}
	}
	exit_shell(memory, is_redir, saved_fds);
}

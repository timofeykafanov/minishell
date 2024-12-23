/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 15:21:31 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

extern volatile sig_atomic_t g_exit_status;

void	execute_pwd(t_memory *memory)
{
	getcwd(memory->pwd, PATH_MAX);
	if (memory->pwd != NULL)
		ft_printf("%s\n", STDOUT_FILENO, memory->pwd);
	else
		perror("pwd");
}

static bool	contains_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && \
			str[i] != '\v' && str[i] != '\f' && str[i] != '\r')
			return (false);
		i++;
	}
	return (true);
}

static bool	contains_only_digits(char *str, int *sign)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || \
		str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
	{
		*sign = -1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			if (contains_only_spaces(str + i))
				return (true);
			return (false);
		}
		i++;
	}
	return (true);
}

static void	exit_shell(t_memory *memory, t_command *cmd, \
	int saved_fds[2])
{
	int	exit_status;
	if (cmd->is_redir_in && saved_fds)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	if (cmd->is_redir_out && saved_fds)
	{
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	exit_status = cmd->exit_status;
	if (saved_fds)
	{
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	if (!memory->is_child)
		ft_printf("exit\n", STDOUT_FILENO);
	free_memory(memory);
	close(1);
	close(0);
	exit(exit_status);
}

static bool is_within_long_range(char *str, int sign)
{
	char	*max_str = "9223372036854775807";
	char	*min_str = "9223372036854775808";
	int		len;

	len = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '+' || *str == '-')
	{
		str++;
	}
	while (*str == '0')
		str++;
	while (str[len])
	{	
		len++;
	}
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t'
		|| str[len - 1] == '\n' || str[len - 1] == '\v'
		|| str[len - 1] == '\f' || str[len - 1] == '\r'))
    	len--;
	if (len > 19)
		return false;
	if (len == 19)
	{
		if (sign == 1)
		{
			for (int i = 0; i < 19; i++)
			{
				if (str[i] > max_str[i])
					return false;
				if (str[i] < max_str[i])
					break ;
			}
		}
		else
		{
			for (int i = 0; i < 19; i++)
			{
				if (str[i] > min_str[i])
					return false;
				if (str[i] < min_str[i])
					break ;
			}
		}
	}
	return true;
}

void	execute_exit(t_memory *memory, t_command *cmd, int saved_fds[2])
{
	int sign = 1;

	if (cmd->args[1] && contains_only_digits(cmd->args[1], &sign) && cmd->args[2])
	{
		ft_printf("exit: too many arguments\n", STDERR_FILENO);
		cmd->exit_status = 1;
		memory->exit_failed = true;
		return;
	}
	else if (cmd->args[1])
	{
		if (contains_only_digits(cmd->args[1], &sign)
			&& is_within_long_range(cmd->args[1], sign)
			&& cmd->args[1][0])
			cmd->exit_status = ft_atoi(cmd->args[1]) % 256;
		else
		{
			ft_printf("exit: %s: numeric argument required\n", STDERR_FILENO, cmd->args[1]);
			cmd->exit_status = 2;
		}
	}
	if (sign == -1)
		cmd->exit_status = (256 + cmd->exit_status) % 256;
	exit_shell(memory, cmd, saved_fds);
}

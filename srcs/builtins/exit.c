/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/09 16:41:38 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_redir_back(t_memory *mem, t_command *cmd, int saved_fds[2])
{
	if (cmd->is_redir_in && saved_fds)
	{
		if (dup2(saved_fds[0], STDIN_FILENO) == -1)
		{
			perror("kinkshell: dup2");
			end_shell(mem);
		}
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	if (cmd->is_redir_out && saved_fds)
	{
		if (dup2(saved_fds[1], STDOUT_FILENO) == -1)
		{
			perror("kinkshell: dup2");
			end_shell(mem);
		}
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	if (saved_fds)
	{
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
}

static void	exit_shell(t_memory *memory, t_command *cmd, \
	int saved_fds[2])
{
	int	exit_status;

	handle_redir_back(memory, cmd, saved_fds);
	exit_status = cmd->exit_status;
	if (!memory->is_child)
		ft_printf("exit\n", STDOUT_FILENO);
	free_memory(memory);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	exit(exit_status);
}

static bool	is_within_long_range(char *str, int sign)
{
	int		len;

	len = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (*str == '0')
		str++;
	while (str[len])
		len++;
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t'
			|| str[len - 1] == '\n' || str[len - 1] == '\v'
			|| str[len - 1] == '\f' || str[len - 1] == '\r'))
		len--;
	if (len > 19)
		return (false);
	if (len == 19)
		return (check_overflow(sign, str));
	return (true);
}

static void	handle_exit_error(t_memory *memory, t_command *cmd)
{
	ft_printf("kinkshell: exit: too many arguments\n", STDERR_FILENO);
	cmd->exit_status = 127;
	memory->exit_failed = true;
}

void	execute_exit(t_memory *memory, t_command *cmd, int saved_fds[2])
{
	int	sign;

	sign = 1;
	if (!cmd->args[1])
		cmd->exit_status = memory->exit_status;
	else if (cmd->args[1] && contains_only_digits(cmd->args[1], &sign)
		&& cmd->args[2])
	{
		handle_exit_error(memory, cmd);
		return ;
	}
	else if (cmd->args[1])
	{
		if (contains_only_digits(cmd->args[1], &sign) && is_within_long_range(\
		cmd->args[1], sign) && cmd->args[1][0] && ft_strlen(cmd->args[1]) > 0)
			cmd->exit_status = ft_atoi(cmd->args[1]) % 256;
		else
		{
			ft_printf(ERROR_MSG_EXIT_NUM, STDERR_FILENO, cmd->args[1]);
			cmd->exit_status = 2;
		}
	}
	if (sign == -1)
		cmd->exit_status = (256 + cmd->exit_status) % 256;
	exit_shell(memory, cmd, saved_fds);
}

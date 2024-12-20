/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:15:57 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 21:25:02 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	contains_slash(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (command[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

bool	is_builtin(char *command)
{
	if (!ft_strncmp(command, ECHO, 5) || !ft_strncmp(command, CD, 3)
		|| !ft_strncmp(command, PWD, 4) || !ft_strncmp(command, EXPORT, 7)
		|| !ft_strncmp(command, UNSET, 6) || !ft_strncmp(command, ENV, 4)
		|| !ft_strncmp(command, EXIT, 5))
		return (true);
	return (false);
}

void	execute_builtin(t_command *cmd, t_memory *mem, bool is_redir_out, \
	bool is_redir_in, int saved_fds[2])
{
	if (!ft_strncmp(cmd->args[0], ECHO, 5))
		echo(cmd->args);
	else if (!ft_strncmp(cmd->args[0], CD, 4))
		execute_cd(mem, cmd);
	else if (!ft_strncmp(cmd->args[0], PWD, 4))
		execute_pwd(mem);
	else if (!ft_strncmp(cmd->args[0], EXPORT, 7))
		execute_export(mem, cmd->args);
	else if (!ft_strncmp(cmd->args[0], UNSET, 6))
		unset(mem, cmd->args);
	else if (!ft_strncmp(cmd->args[0], ENV, 4))
		print_env(mem);
	else if (!ft_strncmp(cmd->args[0], EXIT, 5))
		execute_exit(mem, cmd, is_redir_out, is_redir_in, saved_fds);
}

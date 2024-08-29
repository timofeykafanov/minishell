/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:15:57 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/29 14:17:42 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool	is_builtin(char *command)
{
	if (!ft_strncmp(command, ECHO, 4)
		|| !ft_strncmp(command, CD, 2)
		|| !ft_strncmp(command, PWD, 3) || !ft_strncmp(command, EXPORT, 6)
		|| !ft_strncmp(command, UNSET, 5) || !ft_strncmp(command, ENV, 3)
		|| !ft_strncmp(command, EXIT, 4))
		return (true);
	return (false);
}

void	execute_builtin(t_command *cmd, t_memory *mem)
{
	if (!ft_strncmp(cmd->args[0], ECHO, 4))
		echo(cmd->args);
	else if (!ft_strncmp(cmd->args[0], CD, 2))
		execute_cd(mem, cmd);
	else if (!ft_strncmp(cmd->args[0], PWD, 3))
		execute_pwd(mem);
	else if (!ft_strncmp(cmd->args[0], EXPORT, 6))
		execute_export(mem, cmd->args);
	else if (!ft_strncmp(cmd->args[0], UNSET, 5))
		unset(mem, cmd->args);
	else if (!ft_strncmp(cmd->args[0], ENV, 3))
		print_env(mem);
	else if (!ft_strncmp(cmd->args[0], EXIT, 4))
		execute_exit(mem);
	// exit(0);
}


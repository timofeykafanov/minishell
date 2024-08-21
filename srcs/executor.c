/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 10:39:16 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/21 16:53:50 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_commands(t_memory *memory)
{
	t_commands	*cmd;
	char		*res;
	int			input_fd = -1;

	cmd = memory->commands;
	while (cmd)
	{
		if (cmd->type == T_WORD)
		{
			// if (ft_strncmp(cmd->name, ECHO, ft_strlen(cmd->name)) == 0)
			// 	execute_echo(memory, cmd);
			if (ft_strncmp(cmd->name, CD, ft_strlen(cmd->name)) == 0)
				res = execute_cd(memory, cmd);
			else if (ft_strncmp(cmd->name, PWD, ft_strlen(cmd->name)) == 0)
				res = execute_pwd(memory);
			else if (ft_strncmp(cmd->name, EXPORT, ft_strlen(cmd->name)) == 0)
				print_export(memory);
			// else if (ft_strncmp(cmd->name, UNSET, ft_strlen(cmd->name)) == 0)
			// 	unset(memory, cmd->args);
			else if (ft_strncmp(cmd->name, ENV, ft_strlen(cmd->name)) == 0)
				print_env(memory);
			// else if (ft_strncmp(cmd->name, EXIT, ft_strlen(cmd->name)) == 0)
			// 	ft_exit(memory, cmd);
			else
			{
				res = ft_execve(memory, cmd, input_fd, cmd->next != NULL);
                // if (cmd->next)
                //     input_fd = -1;
			}
		}
		cmd = cmd->next;
	}
	if (res)
		printf("%s\n", res);
}
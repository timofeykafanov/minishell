/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_cd_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/29 16:25:50 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_pwd(t_memory *memory)
{
	if (memory->path != NULL)
		printf("%s\n", memory->path);
	else
		perror("minishell: pwd");
}

void	execute_cd(t_memory *memory, t_command *cmd)
{
	if (cmd->args)
	{
		if (chdir(cmd->args[1]) != 0)
		{
			ft_printf("minishell: %s: %s: ", STDERR_FILENO, \
				cmd->args[0], cmd->args[1]);
			perror("");
		}
	}
	getcwd(memory->path, PATH_MAX);
}

void	execute_exit(t_memory *memory)
{
	free_memory(memory);
	printf("exit\n");
	exit(0);
}

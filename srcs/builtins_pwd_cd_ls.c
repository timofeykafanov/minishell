/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_cd_ls.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/29 13:07:50 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_history(void)
{
	HIST_ENTRY **history_entries = history_list(); // forbidden function
	if (history_entries)
	{
		for (int i = 0; history_entries[i]; i++)
		{
			printf("%d: %s\n", i + 1, history_entries[i]->line);
		}
	}
}

void	execute_pwd(t_memory *memory)
{
	if (memory->path != NULL)
		printf("%s\n", memory->path);
	else
		perror("getcwd() error");
}

void	execute_cd(t_memory *memory, t_command *cmd)
{
	if (cmd->args)
	{
		if (chdir(cmd->args[1]) != 0)
			perror("chdir() error");
	}
	// else
	// {
	// 	if (chdir(memory->path) != 0)
	// 		perror("chdir() error");
	// }
	getcwd(memory->path, PATH_MAX);
}

void	execute_exit(t_memory *memory)
{
	free_memory(memory);
	exit(0);
}

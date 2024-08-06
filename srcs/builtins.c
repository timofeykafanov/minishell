/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/06 15:32:41 by tkafanov         ###   ########.fr       */
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

void execute_pwd(t_memory *memory)
{
	if (memory->path != NULL)
		printf("%s\n", memory->path);
	else
		perror("getcwd() error");
}

void execute_cd(const char *path, t_memory *memory)
{
	if (chdir(path) != 0)
		perror("chdir() error");

	getcwd(memory->path, PATH_MAX);
}

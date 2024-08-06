/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_cd_ls.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/06 17:41:28 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_ls(void)
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

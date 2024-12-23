/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:53:06 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/23 16:19:19 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern sig_atomic_t	g_signal;

static void	run_heredoc(t_memory *memory, t_redir_out *redir, int fd)
{
	char	*line;

	while (1)
	{
		if (g_signal == SIGINT)
			break ;
		line = readline("HEREDOC->");
		if (!line)
		{
			ft_printf(ERROR_MSG_HEREDOC, STDERR_FILENO, redir->file_name);
			break ;
		}
		if (ft_strncmp(line, redir->file_name, \
			ft_strlen(redir->file_name)) == 0
			&& ft_strlen(line) == ft_strlen(redir->file_name))
		{
			free(line);
			break ;
		}
		if (!redir->was_quoted)
			line = heredoc_expander(memory, line);
		if (line)
			ft_printf("%s\n", fd, line);
		free(line);
	}
}

static void	heredoc(t_memory *memory, t_redir_out *redir, int i)
{
	int		fd;
	int		randname;
	char	*filename;

	randname = fork();
	if (randname == 0)
	{
		memory->heredocs_count = i;
		free_memory(memory);
		close(1);
		close(0);
		exit(0);
	}
	set_signals(HEREDOC);
	filename = ft_itoa(randname);
	redir->heredoc_file_name = ft_strjoin("heredoc/", filename);
	memory->heredocs[i] = ft_strdup(redir->heredoc_file_name);
	memory->heredocs[i + 1] = NULL;
	free(filename);
	fd = open(redir->heredoc_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	run_heredoc(memory, redir, fd);
	close(fd);
	set_signals(MAIN);
}

void	execute_heredoc(t_memory *memory)
{
	t_command	*current_cmd;
	t_redir_out	*current_redir;
	int			i;

	i = 0;
	current_cmd = memory->commands;
	while (current_cmd)
	{
		current_redir = current_cmd->redir_struct;
		while (current_redir)
		{
			if (current_redir->type == T_HEREDOC)
			{
				heredoc(memory, current_redir, i);
				i++;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}

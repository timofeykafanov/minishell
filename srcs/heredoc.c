/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:53:06 by sopperma          #+#    #+#             */
/*   Updated: 2024/09/05 17:35:38 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void delete_heredocs(t_memory *memory)
{
	int i = 0;

	while(memory->heredocs[i])
	{
		unlink(memory->heredocs[i]);
		i++;
	}
}

void execute_heredoc(t_memory *memory)
{
	t_command *current_cmd;
	t_redir_out *current_redir;
	int i = 0;

	current_cmd = memory->commands;
	while (current_cmd)
	{
		current_redir = current_cmd->redir_struct;
		while(current_redir)
		{
			if(current_redir->type == T_HEREDOC)
			{
				heredoc(memory, current_redir, i);
				i++;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}
// char	generate_random_file_name(void)
// {
	
// }

char *heredoc_expander(t_memory *memory, char *line)
{
	char *expand_line= NULL;
	char *var;
	int i = 0;

	while(line[i])
	{
		if(line[i] == '$')
		{
			if (ft_isalnum(line[i + 1]) || line[i + 1] == '_')
			{
				var = ft_strndup(&line[i], is_var_end(&line[i + 1]) - &line[i]);
				i+= ft_strlen(var) - 1;
				var = expand_var(memory, var);
			}
			else if (line[i + 1] == '?')
			{
				var = ft_itoa(memory->exit_status);
				i++;
			}
			else
				var = ft_strndup(&line[i], 1);
			expand_line = ft_strljoin(expand_line, var, ft_strlen(var));
			free(var);
		}
		else
		{
			expand_line = ft_strljoin(expand_line, &line[i], 1);
			if(!line)
				return (NULL);
		}
		i++;
	}
	free(line);
	return (expand_line);
}

void	heredoc(t_memory *memory, t_redir_out *redir, int i)
{
	char	*line;
	int		fd;

	int randname = fork();
	if (randname == 0)
		exit(0);
	char *filename = ft_itoa(randname);
	redir->heredoc_file_name = ft_strjoin("heredoc/", filename);
	memory->heredocs[i] = ft_strdup(redir->heredoc_file_name);
	free(filename);
	fd = open(redir->heredoc_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while (1)
	{
		line = readline("HEREDOC->");
		if (line == NULL || ft_strncmp(line, redir->file_name, \
			ft_strlen(redir->file_name)) == 0)
		{
			free(line);
			break ;
		}
		if(!redir->was_quoted)
			line = heredoc_expander(memory, line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
}

char	*read_heredoc_content(void)
{
	int		fd;
	char	*line;
	char	*content;
	bool	flag;

	flag = 0;
	content = NULL;
	fd = open("heredoc.txt", O_RDONLY);
	if (fd == -1)
		return (NULL);
	line = get_next_line(fd, &flag, false);
	if (!line)
		return (NULL);
	while (line != NULL)
	{
		content = ft_strljoin(content, line, ft_strlen(line));
		if (!content)
			return (NULL);
		free(line);
		line = get_next_line(fd, &flag, false);
	}
	close(fd);
	return (content);
}

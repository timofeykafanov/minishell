/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:16:13 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/25 19:16:19 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	delete_heredocs(t_memory *memory)
{
	int	i;

	i = 0;
	while (memory->heredocs[i])
	{
		unlink(memory->heredocs[i]);
		free(memory->heredocs[i]);
		memory->heredocs[i] = NULL;
		i++;
	}
}

static void	expand_exit_status(char **var, t_memory *memory, char *line, int *i)
{
	*var = ft_itoa(memory->exit_status);
	if (!(*var))
	{
		free(line);
		end_shell(memory);
	}
	(*i)++;
}

static char	*expand(char *line, int *i, t_memory *memory)
{
	char	*var;

	if (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_')
	{
		var = ft_strndup(&line[*i], is_var_end(&line[*i + 1]) - &line[*i]);
		if (!var)
		{
			free(line);
			end_shell(memory);
		}
		*i += ft_strlen(var) - 1;
		var = expand_var(memory, var);
	}
	else if (line[*i + 1] == '?')
		expand_exit_status(&var, memory, line, i);
	else
	{
		var = ft_strndup(&line[*i], 1);
		if (!var)
		{
			free(line);
			end_shell(memory);
		}
	}
	return (var);
}

static void	get_var(t_memory *memory, char *line, int *i, char **expand_line)
{
	char	*var;

	var = expand(line, i, memory);
	*expand_line = ft_strljoin(*expand_line, var, ft_strlen(var));
	if (!(*expand_line))
	{
		free(var);
		free(line);
		end_shell(memory);
	}
	free(var);
}

char	*heredoc_expander(t_memory *memory, char *line)
{
	char	*expand_line;
	int		i;

	i = 0;
	expand_line = NULL;
	while (line[i])
	{
		if (line[i] == '$')
			get_var(memory, line, &i, &expand_line);
		else
		{
			expand_line = ft_strljoin(expand_line, &line[i], 1);
			if (!expand_line)
			{
				free(line);
				end_shell(memory);
			}
			if (!line)
				return (free(expand_line), NULL);
		}
		i++;
	}
	free(line);
	return (expand_line);
}

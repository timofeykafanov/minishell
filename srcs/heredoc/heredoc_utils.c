/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:16:13 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 17:02:14 by tkafanov         ###   ########.fr       */
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

static char	*expand(char *line, int *i, t_memory *memory)
{
	char	*var;

	if (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_')
	{
		var = ft_strndup(&line[*i], is_var_end(&line[*i + 1]) - &line[*i]);
		*i += ft_strlen(var) - 1;
		var = expand_var(memory, var);
	}
	else if (line[*i + 1] == '?')
	{
		var = ft_itoa(memory->exit_status);
		(*i)++;
	}
	else
		var = ft_strndup(&line[*i], 1);
	return (var);
}

char	*heredoc_expander(t_memory *memory, char *line)
{
	char	*expand_line;
	char	*var;
	int		i;

	i = 0;
	expand_line = NULL;
	while (line[i])
	{
		if (line[i] == '$')
		{
			var = expand(line, &i, memory);
			expand_line = ft_strljoin(expand_line, var, ft_strlen(var));
			free(var);
		}
		else
		{
			expand_line = ft_strljoin(expand_line, &line[i], 1);
			if (!line)
				return (NULL);
		}
		i++;
	}
	free(line);
	return (expand_line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:43:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/25 18:30:46 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*find_env_value(t_memory *memory, const char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (memory->env[i])
	{
		if (ft_strncmp(memory->env[i], key, key_len) == 0 && memory->env[i][key_len] == '=')
			return (ft_strdup(ft_strchr(memory->env[i], '=') + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*expand_var(t_memory *memory, char *var)
{
	char	*value;
	char	*temp;

	if (*var == '$' && *(var + 1) == '?')
	{
		value = ft_itoa(memory->exit_status);
		free(var);
		return (value);
	}
	else if (*var == '$' && (is_whitespace(var + 1) || ft_strlen(var) == 1))
		return (var);
	temp = ft_strjoin(var + 1, "");
	if (!temp)
	{
		free(var);
		return (NULL);
	}
	value = find_env_value(memory, temp);
	free(temp);
	free(var);
	return (value);
}

t_tokens	*split_var(t_memory *memory, char *var)
{
	char	*value;
	char	*temp;
	t_tokens	*token;

	if (*var == '$' && *(var + 1) == '?')
	{
		value = ft_itoa(memory->exit_status);
		free(var);
		token = variable_split_lexer(memory, value);
		return (token);
	}
	else if (*var == '$' && (is_whitespace(var + 1) || ft_strlen(var) == 1))
	{
		token = variable_split_lexer(memory, var);
		return (token);
	}
	temp = ft_strjoin(var + 1, "");
	if (!temp)
	{
		free(var);
		return (NULL);
	}
	value = find_env_value(memory, temp);
	free(temp);
	free(var);
	token = variable_split_lexer(memory, value);
	free(value);
	return (token);
}

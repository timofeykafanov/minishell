/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:43:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 19:32:07 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_failure(t_memory *memory, char *to_free)
{
	free(to_free);
	end_shell(memory);
}

char	*find_env_value(t_memory *memory, char *key)
{
	int		i;
	size_t	key_len;
	char	*value;

	i = 0;
	if (key)
		key_len = ft_strlen(key);
	else
		key_len = 0;
	while (memory->env[i])
	{
		if (ft_strncmp(memory->env[i], key, key_len) == 0 \
			&& memory->env[i][key_len] == '=')
		{
			value = ft_strdup(ft_strchr(memory->env[i], '=') + 1);
			if (!value)
				handle_failure(memory, key);
			return (value);
		}
		i++;
	}
	value = ft_strdup("");
	if (!value)
		handle_failure(memory, key);
	return (value);
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
		var = NULL;
		return (NULL);
	}
	value = find_env_value(memory, temp);
	free(temp);
	free(var);
	var = NULL;
	return (value);
}

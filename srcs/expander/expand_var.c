/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:43:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 13:24:10 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	find_env_value(t_memory *memory, char *value, char *temp)
{
	int	i;

	i = 0;
	while (memory->env[i])
	{
		if (ft_strncmp(memory->env[i], temp, ft_strlen(temp)) == 0)
		{
			free(value);
			value = ft_strdup(ft_strchr(memory->env[i], '=') + 1);
			break ;
		}
		i++;
	}
}

// TODO: protect itoa

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
	value = malloc(1);
	value[0] = '\0';
	temp = ft_strjoin(var, "=");
	temp++;
	find_env_value(memory, value, temp);
	free(temp - 1);
	free(var);
	var = NULL;
	return (value);
}

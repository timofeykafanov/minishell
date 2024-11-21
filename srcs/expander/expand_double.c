/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:44:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/21 15:37:56 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_var_expansion(t_memory *memory, char **s, \
	char **res, char **var)
{
	*var = ft_strndup(*s, is_var_end(*s + 1) - *s);
	if (!*var)
		return (false);
	if ((*var)[ft_strlen(*var) - 1] == '"')
		(*var)[ft_strlen(*var) - 1] = '\0';
	*var = expand_var(memory, *var);
	if (!*var)
		return (false);
	*res = ft_strljoin(*res, *var, ft_strlen(*var));
	free(*var);
	*s = is_var_end(*s + 1);
	return (true);
}

static char	*create_null_string(char **res)
{
	*res = malloc(1);
	(*res)[0] = '\0';
	return (*res);
}

char	*reduce_spaces(char *str)
{
	char *result;
	size_t i = 0, j = 0;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	if (str[i] == ' ')
        result[j++] = str[i++];
	while (str[i])
	{
		if (str[i] == ' ' && (j == 0 || result[j - 1] == ' '))
		{
			i++;
			continue;
		}
		result[j++] = str[i++];
	}
	// if (j > 0 && result[j - 1] == ' ')
	// 	j--;
	result[j] = '\0';
	return (result);
}
// TODO: add length to pointer and free var
char	*expand_double(t_memory *memory, char *s)
{
	char	*res;
	char	*var;

	res = NULL;
	if (is_double_quote(*s) && is_double_quote(*(s + 1)))
		return (create_null_string(&res));
	s++;
	while (*s)
	{
		if (is_dollar(*s))
		{
			if (!handle_var_expansion(memory, &s, &res, &var))
				return (NULL);
		}
		else
		{
			if (is_double_quote(*s))
				break ;
			res = ft_strljoin(res, s, 1);
			s++;
		}
		if (!res)
			return (NULL);
	}
	res = reduce_spaces(res);
	return (res);
}

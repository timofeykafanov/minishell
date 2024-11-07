/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:44:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 15:54:53 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_var_expansion(t_memory *memory, char **s, \
	char **res, char **var)
{
	*var = ft_strndup(*s, is_var_end(*s + 1) - *s);
	if (!*var)
		return (false);
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
	return (res);
}

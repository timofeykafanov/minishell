/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:44:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 14:35:49 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_var_expansion(t_memory *memory, char **s, \
	char **res)
{
	char	*var;

	var = ft_strndup(*s, is_var_end(*s + 1) - *s);
	if (!var)
		return (false);
	if ((var)[ft_strlen(var) - 1] == '"')
		(var)[ft_strlen(var) - 1] = '\0';
	if ((var && var[0] && var[1] && !is_separator(var[1])) \
		|| ft_strncmp(var, "$?", 2) == 0)
		var = expand_var(memory, var);
	if (!var)
		return (false);
	*res = ft_strljoin(*res, var, ft_strlen(var));
	free(var);
	*s = is_var_end(*s + 1);
	return (true);
}

// TODO: add length to pointer and free var
char	*expand_double(t_memory *memory, char *s)
{
	char	*res;

	res = NULL;
	if (is_double_quote(*s) && is_double_quote(*(s + 1)))
		return (ft_strdup(""));
	s++;
	while (*s)
	{
		if (is_dollar(*s))
		{
			if (!handle_var_expansion(memory, &s, &res))
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

bool	expand_double_quotes(t_tokens *token, t_memory *memory)
{
	char	*data;

	if (is_prev_heredoc(token))
	{
		token->data = remove_quotes(token->data);
		if (!token->data)
			end_shell(memory);
		token->was_quoted = 1;
		return (true);
	}
	data = token->data;
	token->data = expand_double(memory, token->data);
	free(data);
	if (!token->data)
		end_shell(memory);
	token->was_quoted = 1;
	return (true);
}

char	*remove_quotes(char *s)
{
	char	*res;
	char	*orginal_data;

	res = NULL;
	orginal_data = s;
	if (is_double_quote(*s) && is_double_quote(*(s + 1)))
	{
		free(orginal_data);
		return (ft_strdup(""));
	}
	s++;
	while (*s)
	{
		if (is_double_quote(*s) || is_single_quote(*s))
			break ;
		res = ft_strljoin(res, s, 1);
		if (!res)
			return (NULL);
		s++;
	}
	free(orginal_data);
	return (res);
}

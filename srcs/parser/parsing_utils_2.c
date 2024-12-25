/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:29:25 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/25 20:03:05 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirect(int type)
{
	return (type == T_R_OUT || type == T_OUT_APPEND \
		|| type == T_R_IN || type == T_HEREDOC);
}

int	is_whitespace(char *s)
{
	if (*s == ' ' || *s == '\t' || *s == '\n')
		return (1);
	else
		return (0);
}

char	*find_whitespace(char *s)
{
	while (*s)
	{
		if (*s == ' ' || *s == '\t' || *s == '\n')
			return (s);
		else
			s++;
	}
	return (NULL);
}

bool	is_separator(char c)
{
	return ((c >= '!' && c <= '/')
		|| (c >= ':' && c <= '@')
		|| (c >= '[' && c <= '^')
		|| (c >= '{' && c <= '~'));
}

char	*is_var_end(char *s)
{
	int	i;

	i = 1;
	while (s[i])
	{
		if (is_separator(s[i]) || is_whitespace(s + i))
			break ;
		i++;
	}
	return (s + i);
}

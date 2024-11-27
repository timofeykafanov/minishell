/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 13:29:25 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/27 16:23:40 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

//WHY is this an infinte loop if the first char is a $?
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

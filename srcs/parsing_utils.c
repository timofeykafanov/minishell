/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:57 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/06 16:11:52 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int is_whitespace(char *s)
{
	if(*s == ' ' || *s == '\t' || *s == '\n')
		return (1);
	else
		return (0);
}

static int is_metacharacter(char *s)
{
	if(*s == PIPE || *s == R_IN || *s  == R_OUT \
		|| *s == SEMICOLON || *s == BRACKET_O  \
		|| *s  == BRACKET_C || *s == AMPERSAND \
		|| is_whitespace(s))
		return (1);
	else
		return (0);
}

char *skip_whitespace(char *s)
{
	int i;

	i = 0;
	while(*s && is_whitespace(s))
	{
		s++;
		i++;
	}
	return (s);
}

int skip_non_whitespace(char *s)
{
	int i;

	i = 0;
	while(*s && !is_whitespace(s))

	{
		s++;
		i++;
	}
	return (i);
}

char	*find_seperator(char *s)
{
	while (*s && !is_metacharacter(s))
		s++;
	return (s);
}

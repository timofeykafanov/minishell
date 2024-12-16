/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:27:03 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/16 15:57:54 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_quotes(void **token, int *len, char *s, t_memory *memory)
{
	if (*s == D_QUOTE)
	{
		if (!ft_strchr((s + 1), D_QUOTE))
			return (set_error_code(LEXER, ERROR_CODE_QUOTES, memory), false);
		*len = ft_strchr((s + 1), D_QUOTE) - s + 1;
		*token = ft_strncpy(s, *len);
	}
	else if (*s == S_QUOTE)
	{
		if (!ft_strchr((s + 1), S_QUOTE))
			return (set_error_code(LEXER, ERROR_CODE_QUOTES, memory), false);
		*len = ft_strchr((s + 1), S_QUOTE) - s + 1;
		*token = ft_strncpy(s, *len);
	}
	return (true);
}

static void	handle_dash_dollar(void **token, int *len, char *s)
{
	if (*s == DASH)
	{
		*len = find_seperator(s) - s;
		*token = ft_strncpy(s, *len);
	}
	else if (*s == DOLLAR)
	{
		if (ft_strncmp(s, "$?", 2) == 0)
			*len = 2;
		else
			*len = is_var_end(s) - s;
		*token = ft_strncpy(s, *len);
	}
}

static void	handle_redir_whitespace(void **token, int *len, char *s)
{
	if ((*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT))
	{
		*len = 2;
		*token = ft_strncpy(s, *len);
	}
	else if (is_whitespace(s))
	{
		*len = skip_whitespace(s) - s;
		*token = ft_strncpy(s, *len);
	}
}

static void	handle_group(void **token, int *len, char *s)
{
	if (*s == DASH || *s == DOLLAR)
		handle_dash_dollar(token, len, s);
	else if (*s == PIPE || *s == SEMICOLON || (*s == R_IN && *(s + 1) != R_IN)
		|| (*s == R_OUT && *(s + 1) != R_OUT))
	{
		*len = 1;
		*token = ft_strncpy(s, *len);
	}
	else if ((*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT)
		|| is_whitespace(s))
		handle_redir_whitespace(token, len, s);
}

static bool	is_group_identifier(char *s)
{
	return (*s == DASH || *s == DOLLAR || *s == PIPE || *s == SEMICOLON
		|| (*s == R_IN && *(s + 1) != R_IN)
		|| (*s == R_OUT && *(s + 1) != R_OUT)
		|| (*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT));
}

void *process_token(char *s, t_memory *memory, bool split)
{
	void	*token;
	int		len;

	token = NULL;
	len = 0;
	if ((*s == D_QUOTE || *s == S_QUOTE) && !split)
	{
		if (!handle_quotes(&token, &len, s, memory))
			return (NULL);
	}
	else if ((is_group_identifier(s) && !split) || is_whitespace(s))
		handle_group(&token, &len, s);
	else
	{
		if (split)
			len = skip_non_whitespace(s);
		else
		{
			len = find_seperator(s) - s;
			if (*find_seperator(s) == EQUALS)
				len += 1;
		}
		token = ft_strncpy(s, len);
	}
	return (token);
}

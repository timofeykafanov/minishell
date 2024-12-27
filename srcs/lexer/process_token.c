/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:27:03 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 21:10:06 by tkafanov         ###   ########.fr       */
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
		if (!*token)
			return (false);
	}
	else if (*s == S_QUOTE)
	{
		if (!ft_strchr((s + 1), S_QUOTE))
			return (set_error_code(LEXER, ERROR_CODE_QUOTES, memory), false);
		*len = ft_strchr((s + 1), S_QUOTE) - s + 1;
		*token = ft_strncpy(s, *len);
		if (!*token)
			return (false);
	}
	return (true);
}

static bool	handle_dollar(void **token, int *len, char *s)
{
	if (*s == DOLLAR)
	{
		if (ft_strncmp(s, "$?", 2) == 0)
			*len = 2;
		else
			*len = is_var_end(s) - s;
		*token = ft_strncpy(s, *len);
		if (!*token)
			return (false);
	}
	return (true);
}

static bool	handle_redir_whitespace(void **token, int *len, char *s)
{
	if ((*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT))
	{
		*len = 2;
		*token = ft_strncpy(s, *len);
		if (!*token)
			return (false);
	}
	else if (is_whitespace(s))
	{
		*len = skip_whitespace(s) - s;
		*token = ft_strncpy(s, *len);
		if (!*token)
			return (false);
	}
	return (true);
}

static bool	handle_group(void **token, int *len, char *s)
{
	if (*s == DOLLAR)
	{
		if (!handle_dollar(token, len, s))
			return (false);
	}
	else if (*s == PIPE || (*s == R_IN && *(s + 1) != R_IN)
		|| (*s == R_OUT && *(s + 1) != R_OUT))
	{
		*len = 1;
		*token = ft_strncpy(s, *len);
		if (!*token)
			return (false);
	}
	else if ((*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT)
		|| is_whitespace(s))
	{
		if (!handle_redir_whitespace(token, len, s))
			return (false);
	}
	return (true);
}

void	*process_token(char *s, t_memory *memory, bool split)
{
	void	*token;

	token = NULL;
	memory->len = 0;
	if ((*s == D_QUOTE || *s == S_QUOTE) && !split)
	{
		if (!handle_quotes(&token, &memory->len, s, memory))
			return (NULL);
	}
	else if ((is_group_identifier(s) && !split) || is_whitespace(s))
	{
		if (!handle_group(&token, &memory->len, s))
			return (NULL);
	}
	else
	{
		if (split)
			memory->len = skip_non_whitespace(s);
		else
			check_separator(s, &memory->len);
		token = ft_strncpy(s, memory->len);
		if (!token)
			return (NULL);
	}
	return (token);
}

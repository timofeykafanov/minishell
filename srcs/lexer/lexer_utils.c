/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:28:46 by sopperma          #+#    #+#             */
/*   Updated: 2025/01/08 16:20:18 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	sanitize_tokens (t_memory *memory)
{
	// TODO: sanitize_tokens shouldn't remove empty qoutes
	t_tokens	*token;
	t_tokens	*next;

	token = memory->tokens;
	while (token)
	{
		if (!token->data || ft_strlen(token->data) == 0 )
		{
			if (token->type == T_D_QUOTE || token->type == T_S_QUOTE)
			{
				token = token->next;
				continue;
			}
			if (token->prev)
				token->prev->next = token->next;
			else
			{
				if (!token->next)
					break;
				memory->tokens = token->next;
			}
			if (token->next && token->prev)
				token->next->prev = token->prev;
			next = token->next;
			free(token->data);
			free(token);
			token = next;
			continue;
		}
		token = token->next;
	}
}

bool	is_group_identifier(char *s)
{
	return (*s == DOLLAR || *s == PIPE
		|| (*s == R_IN && *(s + 1) != R_IN)
		|| (*s == R_OUT && *(s + 1) != R_OUT)
		|| (*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT));
}

int	get_type(char *s)
{
	if (*s == D_QUOTE)
		return (T_D_QUOTE);
	else if (*s == S_QUOTE)
		return (T_S_QUOTE);
	else if (*s == DOLLAR)
		return (T_VAR);
	else if ((*s == R_IN && *(s + 1) != R_IN))
		return (T_R_IN);
	else if ((*s == R_OUT && *(s + 1) != R_OUT))
		return (T_R_OUT);
	else if ((*s == R_IN && *(s + 1) == R_IN))
		return (T_HEREDOC);
	else if ((*s == R_OUT && *(s + 1) == R_OUT))
		return (T_OUT_APPEND);
	else if (*s == PIPE)
		return (T_PIPE);
	else if (is_whitespace(s))
		return (T_WHITESPACE);
	else
		return (T_WORD);
}

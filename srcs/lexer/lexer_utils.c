/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:28:46 by sopperma          #+#    #+#             */
/*   Updated: 2025/01/21 11:56:10 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	handle_empty_token_links(t_memory *memory, t_tokens **token)
{
	t_tokens	*current;
	t_tokens	*next;

	current = *token;
	next = current->next;
	if (current->prev)
		current->prev->next = next;
	else
	{
		if (!next)
		{
			current->type = T_WHITESPACE;
			return (true);
		}
		memory->tokens = next;
	}
	if (next && current->prev)
		next->prev = current->prev;
	free(current->data);
	free(current);
	*token = next;
	return (false);
}

void	sanitize_tokens(t_memory *memory)
{
	t_tokens	*token;

	token = memory->tokens;
	while (token)
	{
		if (!token->data || ft_strlen(token->data) == 0)
		{
			if (token->type == T_D_QUOTE || token->type == T_S_QUOTE || 
				(token->type == T_WORD && token->was_quoted))
			{
				token = token->next;
				continue ;
			}
			if (handle_empty_token_links(memory, &token))
				break ;
			continue ;
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

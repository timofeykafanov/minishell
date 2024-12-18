/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:28:46 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/18 17:52:48 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_tokens(t_memory *memory)
{
	t_tokens	*current;
	int			i;

	i = 1;
	current = memory->tokens;
	printf("Tokens:\n");
	while (current)
	{
		if (current->was_quoted)
			printf("Nr: %d\nToken: %s W/O Quotes: %s Type: \
			%d Quoted : %d\n", i++, (char *)current->data, \
			(char *)current->quotes_removed, current->type, \
			current->was_quoted);
		else
			printf("Nr: %d\nToken: %s Type: %d Quoted : %d\n", i++, \
			(char *)current->data, current->type, current->was_quoted);
		current = current->next;
	}
	return (1);
}

bool	is_group_identifier(char *s)
{
	return (*s == DASH || *s == DOLLAR || *s == PIPE || *s == SEMICOLON
		|| (*s == R_IN && *(s + 1) != R_IN)
		|| (*s == R_OUT && *(s + 1) != R_OUT)
		|| (*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT));
}

int	print_expand_var_tokens(t_tokens *token)
{
	t_tokens	*current;
	int			i;

	i = 1;
	current = token;
	printf("Tokens:\n");
	while (current)
	{
		if (current->was_quoted)
			printf("Nr: %d\nToken: %s W/O Quotes: \
			%s Type: %d Quoted : %d\n", i++, (char *)current->data, \
			(char *)current->quotes_removed, current->type, \
			current->was_quoted);
		else
			printf("Nr: %d\nToken: %s Type: %d Quoted : %d\n", i++, \
			(char *)current->data, current->type, current->was_quoted);
		current = current->next;
	}
	return (1);
}

int	get_type(char *s)
{
	if (*s == D_QUOTE)
		return (T_D_QUOTE);
	else if (*s == S_QUOTE)
		return (T_S_QUOTE);
	else if (*s == DASH)
		return (T_OPTION);
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
	else if (*s == SEMICOLON)
		return (T_SEMICOLON);
	else if (*s == PIPE)
		return (T_PIPE);
	else if (is_whitespace(s))
		return (T_WHITESPACE);
	else
		return (T_WORD);
}

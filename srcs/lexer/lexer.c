/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:22:27 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/11 13:54:06 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_tokens	*create_token(char *s, t_memory *memory)
{
	t_tokens	*token;
	
	token = malloc(sizeof(t_tokens));
	if (!token)
		return (set_error_code(LEXER, ERROR_CODE_MALLOC, memory), NULL);
	token->data = process_token(s, memory);
	if (memory->lexer_error_code == ERROR_CODE_QUOTES)
		return (print_error_message(LEXER, memory), NULL);
	else if (!token->data)
		return (set_error_code(LEXER, ERROR_CODE_MALLOC, memory), NULL);
	token->type = get_type((char *)token->data);
	if (token->type == T_WORD && ft_strchr((char *)token->data, '='))
		token->type = T_VAR_DEF;
	token->was_quoted = 0;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	lexer(t_memory *memory)
{
	char		*input;
	t_tokens	*current;
	t_tokens	*previous;

	current = NULL;
	previous = NULL;
	input = memory->input;
	while (*input)
	{
		current = create_token(input, memory);
		if (!current)
			return ;	
		if (!memory->tokens)
			memory->tokens = current;
		else
		{
			previous->next = current;
			current->prev = previous;
		}
		previous = current;
		input += ft_strlen(current->data);
	}
	return;
}

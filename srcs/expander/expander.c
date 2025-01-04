/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2025/01/04 17:00:49 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	split_variable(char *var_content, t_tokens *token, t_memory *memory)
{
	t_tokens	*start;
	t_tokens	*next;

	start = variable_split_lexer(memory, var_content);
	next = token->next;
	if (!start)
		return (false);
	if (token->prev)
	{
		token->prev->next = start;
		start->prev = token->prev;
	}
	else
		memory->tokens = start;
	free(token->data);
	free(token);
	while (start->next)
		start = start->next;
	start->next = next;
	if (next)
		next->prev = start;
	return (true);
}

bool	handle_var(char *var_content, t_tokens *token, t_memory *memory)
{
	if (handle_redirect(token, memory))
		return (true);
	var_content = expand_var(memory, token->data);
	token->data = var_content;
	if (!var_content)
		return (false);
	// if (ft_strlen(var_content) == 0 && !token->next)
	// {
	// 	token->type = T_WHITESPACE;
	// 	return (true);
	// }
	// if (ft_strlen(var_content) == 0 && token->next)
	if (ft_strlen(var_content) == 0)
	{
		// if (token->prev)
		// 	token->prev->next = token->next;
		// else
		// 	memory->tokens = token->next;
		// if (token->next && token->prev)
		// 	token->next->prev = token->prev;
		free(token->data);
		// free(token);
		token->data = ft_strdup("");
		if (!token->data)
			return (false);
		return (true);
	}
	return (split_variable(var_content, token, memory));
}

static bool	check_token_type(t_tokens *token, t_memory *memory)
{
	char		*var_content;

	var_content = NULL;
	if (token->type == T_D_QUOTE)
		return (expand_double_quotes(token, memory));
	else if (token->type == T_S_QUOTE)
	{
		token->data = expand_single(token->data);
		if (!token->data)
			end_shell(memory);
		token->was_quoted = 1;
	}
	else if (token->type == T_VAR)
		return (handle_var(var_content, token, memory));
	return (true);
}

void	*expand_tokens(t_memory *memory)
{
	t_tokens	*token;
	t_tokens	*next;

	token = memory->tokens;
	while (token)
	{
		next = token->next;
		if (!check_token_type(token, memory))
			end_shell(memory);
		token = next;
	}
	merge_tokens(memory);
	return (memory);
}

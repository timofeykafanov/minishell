/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/25 15:44:37 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	to_merge(t_tokens	*current_token)
{
	return (current_token->prev && (current_token->type == T_WORD
			|| current_token->type == T_OPTION
			|| current_token->type == T_D_QUOTE
			|| current_token->type == T_S_QUOTE
			|| current_token->type == T_VAR
			|| current_token->type == T_VAR_DEF)
		&& (current_token->prev->type == T_WORD
			|| current_token->prev->type == T_OPTION
			|| current_token->prev->type == T_D_QUOTE
			|| current_token->prev->type == T_S_QUOTE
			|| current_token->prev->type == T_VAR
			|| current_token->prev->type == T_VAR_DEF));
}

// TODO: protect join

// static void	merge(t_tokens	*current_token)
// {
// 	char		*data;
// 	t_tokens	*temp;

// 	data = current_token->prev->data;
// 	printf("current_token->data: %s\n", (char*)current_token->data);
// 	printf("current_token->prev->data: %s\n\n", (char*)current_token->prev->data);
	
// 	current_token->prev->data = ft_strjoin(current_token->prev->data,
// 	current_token->data);
// 	free(data);
// 	current_token->prev->type = T_WORD;
// 	current_token->prev->was_quoted = 1;
// 	current_token->prev->next = current_token->next;
// 	if (current_token->next)
// 		current_token->next->prev = current_token->prev;
// 	free(current_token->data);
// 	temp = current_token;
// 	current_token = current_token->next;
// 	free(temp);
// }

static	t_tokens	*merge(t_tokens	*current_token)
{
	char		*data;
	t_tokens	*temp;

	data = current_token->prev->data;
		
	current_token->prev->data = ft_strjoin(current_token->prev->data, \
	current_token->data);
	free(data);
	current_token->prev->type = T_WORD;
	current_token->prev->was_quoted = 1;
	current_token->prev->next = current_token->next;
	if (current_token->next)
		current_token->next->prev = current_token->prev;
	// free(current_token->data);
	temp = current_token;
	current_token = current_token->next;
	free(temp);
	return current_token;
}

//  TODO: merge current if before is word and no whitespace 

void	merge_tokens(t_memory *memory)
{
	t_tokens	*current_token;

	current_token = memory->tokens;
	while (current_token)
	{
		if (to_merge(current_token))
			current_token = merge(current_token);

		else
			current_token = current_token->next;
	}
}

static bool	check_token_type(t_tokens *token, t_memory *memory)
{
	if (token->type == T_D_QUOTE)
	{
		token->quotes_removed = remove_quotes(token->data);
		token->data = expand_double(memory, token->data);
		if (!token->data)
			return (false);
		token->was_quoted = 1;
	}
	else if (token->type == T_S_QUOTE)
	{
		token->data = expand_single(token->data);
		if (!token->data)
			return (false);
		token->was_quoted = 1;
	}
	else if (token->type == T_VAR)
	{
		token->data = expand_var(memory, token->data);
		if (!token->data)
			return (false);
	}
	return (true);
}

void	*expand_tokens(t_memory *memory)
{
	t_tokens	*token;

	token = memory->tokens;
	while (token)
	{
		if (!check_token_type(token, memory))
			return (NULL);
		token = token->next;
	}
	merge_tokens(memory);
	return (memory);
}

// void	print_tokens_as_string(t_memory *memory)
// {
// 	t_tokens	*token;

// 	token = memory->tokens;
// 	while (token)
// 	{
// 		printf("%s", (char *)token->data);
// 		token = token->next;
// 	}
// 	printf("\n");
// }

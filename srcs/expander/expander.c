/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/12 14:43:24 by sopperma         ###   ########.fr       */
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
	char		*prev_token_old_data;
	t_tokens	*to_be_freed_token;

	prev_token_old_data = current_token->prev->data;
		
	current_token->prev->data = ft_strjoin(current_token->prev->data, \
	current_token->data);
	free(prev_token_old_data);
	current_token->prev->type = T_WORD;
	current_token->prev->was_quoted = 1;
	current_token->prev->next = current_token->next;
	if (current_token->next)
		current_token->next->prev = current_token->prev;
	to_be_freed_token = current_token;
	// printf("freed merge %p %s\n", (void *)to_be_freed_token, (char *)to_be_freed_token->data);
	free(current_token->data);
	current_token = current_token->next;
	free(to_be_freed_token);
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
	char		*var_content;
	t_tokens	*next;
	t_tokens	*start;
	char		*data;
	char		*expanded_var;
	
	if (token->type == T_D_QUOTE)
	{
		if ((token->prev && token->prev->type == T_HEREDOC)
		|| (token->prev && token->prev->type == T_WHITESPACE && token->prev->prev
		&& token->prev->prev->type == T_HEREDOC))
		{
			token->data = remove_quotes(token->data);
			token->was_quoted = 1;
			return (true);
		}
		data = token->data;
		// token->quotes_removed = remove_quotes(token->data);
		token->data = expand_double(memory, token->data);
		free(data);
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
	// else if (token->type == T_VAR)
	// {
	// 	token->data = expand_var(memory, token->data);
	// 	if (!token->data)
	// 		return (false);
	// }
	else if (token->type == T_VAR)
	{
		if ((token->prev && (token->prev->type == T_HEREDOC))
			|| (token->prev && token->prev->type == T_WHITESPACE && token->prev->prev
			&& (token->prev->prev->type == T_HEREDOC)))
			return (true);
		
		if ((token->prev && (token->prev->type == T_R_OUT
			|| token->prev->type == T_R_IN || token->prev->type == T_OUT_APPEND))
			|| (token->prev && token->prev->type == T_WHITESPACE && token->prev->prev
			&& (token->prev->prev->type == T_R_OUT
			|| token->prev->prev->type == T_R_IN || token->prev->prev->type == T_OUT_APPEND)))
		{
			expanded_var = find_env_value(memory, token->data + 1);
			// printf("%d\n", ft_strncmp(expanded_var, "", 1) == 0);
			if (ft_strncmp(expanded_var, "", 1) == 0)
			{
				set_error_code(EXPANDER, ERROR_CODE_AMBIGUOUS_REDIRECT, memory);
				memory->exit_status = 1;
				if (!memory->faulty_variable_name)
					memory->faulty_variable_name = ft_strdup(token->data);
				return (true);
			}
			free(expanded_var);
		}
		var_content = expand_var(memory, token->data);
		// printf("var_content: %s\n", var_content);
		// printf("1var_content: %s\n", var_content);
		// free(token->data);
		token->data = var_content;
		if (!var_content)
			return (false);
		// printf("var_content: %s\n", var_content);
		
		if (ft_strlen(var_content) == 0 && token->next)
		{
			// free(var_content);
			// printf("freed expander2 %p %s\n", (void *)token, (char *)token->data);
			if (token->prev)
				token->prev->next = token->next;
			else
				memory->tokens = token->next;
			if (token->next && token->prev)
				token->next->prev = token->prev;
			free(token->data);
			free(token);
			return (true);
		}
		
		// printf("2var_content: %s\n", var_content);
		next = token->next;
		if (ft_strlen(var_content) == 0 && !next)
		{
			// TODO: check for leaks (var_content)
			// token->data = var_content;
			token->type = T_WHITESPACE;
			// free(var_content);
			return (true);
		}
		start = variable_split_lexer(memory, var_content);
		// print_expand_var_tokens(start);
		if (!start)
			return (false);
		if (token->prev)
		{
			token->prev->next = start;
			start->prev = token->prev;
		}
		else
		{
			memory->tokens = start;
		}
		// printf("freed expander %p %s\n", (void *)token, (char *)token->data);
		free(token->data);
		free(token);
		while (start->next)
			start = start->next;
		start->next = next;
		if (next)
			next->prev = start;
	}
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
			return (NULL);
		token = next;
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

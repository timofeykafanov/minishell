/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/19 11:24:13 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	handle_redirect(t_tokens *token, t_memory *memory)
{
	char	*expanded_var;

	expanded_var = NULL;
	if (is_prev_heredoc(token))
		return (true);
	if (is_prev_redirect(token))
	{
		expanded_var = find_env_value(memory, token->data + 1);
		if (ft_strncmp(expanded_var, "", 1) == 0)
		{
			set_error_code(EXPANDER, ERROR_CODE_AMBIGUOUS_REDIRECT, memory);
			memory->exit_status = 1;
			if (!memory->faulty_variable_name)
				memory->faulty_variable_name = ft_strdup(token->data);
			free(expanded_var);
			return (true);
		}
		free(expanded_var);
	}
	return (false);
}

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
	if (ft_strlen(var_content) == 0 && !token->next)
	{
		token->type = T_WHITESPACE;
		return (true);
	}
	if (ft_strlen(var_content) == 0 && token->next)
	{
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
			return (false);
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
			return (NULL);
		token = next;
	}
	merge_tokens(memory);
	return (memory);
}

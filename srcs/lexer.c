/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:22:27 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/05 09:59:39 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	*process_token(char *s)
{
	void	*token;
	int		len;

	token = NULL;
	if (*s == '\"')
	{
		if (!ft_strchr((s + 1), '\"'))
			return (NULL);
		len = ft_strchr((s + 1), '\"') - s + 1;
		token = ft_strncpy(s, len);
	}
	else if (*s == '\'')
	{
		if (!ft_strchr((s + 1), '\''))
			return (NULL);
		len = ft_strchr((s + 1), '\'') - s + 1;
		token = ft_strncpy(s, len);
	}
	else if (*s == '(')
	{
		if (!ft_strchr((s + 1), ')'))
			return (NULL);
		len = ft_strchr((s + 1), ')') - s + 1;
		token = ft_strncpy(s, len);
	}
	else if (*s == '-')
	{
		len = find_seperator(s) - s;
		token = ft_strncpy(s, len);
	}
	else if (*s == '$')
	{
		if (ft_strncmp(s, "$?", 2) == 0)
			len = 2;
		else
			len = is_var_end(s) - s;
		token = ft_strncpy(s, len);
	}
	else if (*s == PIPE || *s == SEMICOLON || (*s == R_IN && *(s + 1) != R_IN)
		|| (*s == R_OUT && *(s + 1) != R_OUT))
	{
		len = 1;
		token = ft_strncpy(s, len);
	}
	else if ((*s == R_IN && *(s + 1) == R_IN)
		|| (*s == R_OUT && *(s + 1) == R_OUT))
	{
		len = 2;
		token = ft_strncpy(s, len);
	}
	else if (is_whitespace(s))
	{
		len = skip_whitespace(s) - s;
		token = ft_strncpy(s, len);
	}
	else
	{
		len = find_seperator(s) - s;
		if(*find_seperator(s) == EQUALS)
			len += 1;
		token = ft_strncpy(s, len);
	}
	return (token);
}

static t_tokens	*create_token(char *s, t_memory *memory)
{
	t_tokens	*token;

	token = malloc(sizeof(t_tokens));
	if (!token)
		return (free_memory(memory), NULL);
	token->data = process_token(s);
	if (!token->data)
		return (free_memory(memory), NULL);
	token->type = get_type((char *)token->data);
	if (token->type == T_WORD && ft_strchr((char *)token->data, '='))
		token->type = T_VAR_DEF;
	token->was_quoted = 0;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

int	lexer(t_memory *memory)
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
			return (free_memory(memory), ERROR);
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
	return (SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/09/05 15:55:26 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_single_quote(char c)
{
	return (c == '\'');
}

int	is_double_quote(char c)
{
	return (c == '\"');
}

int	is_dollar(char c)
{
	return (c == '$');
}

char	*expand_var(t_memory *memory, char *var)
{
	char	*value;
	int		i;
	char	*temp;

	i = 0;
	// if (ft_strncmp(var, "$?", 2) == 0)
	if (*var == '$' && *(var + 1) == '?')
	{
		value = ft_itoa(memory->exit_status);
		free(var);
		return (value);
	}
	value = malloc(1);
	value[0] = '\0';
	temp = ft_strjoin(var, "=");
	temp++;
	while (memory->env[i])
	{
		if (ft_strncmp(memory->env[i], temp, ft_strlen(temp)) == 0)
		{
			free(value);
			value = ft_strdup(ft_strchr(memory->env[i], '=') + 1);
			break ;
		}
		i++;
	}
	free(temp - 1);
	free(var);
	var = NULL;
	return (value);
}

//todo add length to pointer and free var
char	*expand_double(t_memory *memory, char *s)
{
	char	*res;
	char	*var;

	res = NULL;
	if (is_double_quote(*s) && is_double_quote(*(s + 1)))
	{
		res = malloc(1);
		res[0] = '\0';
		return (res);
	}
	s++;
	while (*s)
	{
		if (is_dollar(*s))
		{
			var = ft_strndup(s, is_var_end(s + 1) - s);
			if (!var)
				return (NULL);
			var = expand_var(memory, var);
			if (!var)
				return (NULL);
			res = ft_strljoin(res, var, ft_strlen(var));
			free(var);
			s = is_var_end(s + 1);
		}
		else
		{
			if (is_double_quote(*s))
				break ;
			res = ft_strljoin(res, s, 1);
			s++;
		}
		if (!res)
			return (NULL);
	}
	return (res);
}

char	*expand_single(char *s)
{
	char	*res;

	res = NULL;
	if (is_single_quote(*s) && is_single_quote(*(s + 1)))
	{
		res = malloc(1);
		res[0] = '\0';
		return (res);
	}
	s++;
	while (*s)
	{
		if (is_single_quote(*s))
			break ;
		res = ft_strljoin(res, s, 1);
		if (!res)
			return (NULL);
		s++;
	}
	return (res);
}

void	merge_tokens(t_memory *memory)
{
	t_tokens	*current_token;
	t_tokens	*temp;

	// merge current if before is word and no whitespace 
	current_token = memory->tokens;
	while (current_token)
	{
		if (current_token->prev
			&& (current_token->type == T_WORD || current_token->type == T_OPTION \
			|| current_token->type == T_D_QUOTE || current_token->type == T_S_QUOTE \
			|| current_token->type == T_VAR || current_token->type == T_VAR_DEF) \
			&& \
			(current_token->prev->type == T_WORD || current_token->prev->type == T_OPTION \
			|| current_token->prev->type == T_D_QUOTE || current_token->prev->type == T_S_QUOTE \
			|| current_token->prev->type == T_VAR || current_token->prev->type == T_VAR_DEF))
		{
			current_token->prev->data = ft_strjoin(current_token->prev->data, current_token->data);
			current_token->prev->type = T_WORD;
			current_token->prev->was_quoted = 1;
			current_token->prev->next = current_token->next;
			if (current_token->next)
                current_token->next->prev = current_token->prev;
			free(current_token->data);
			temp = current_token;
			current_token = current_token->next;
			free(temp);
		}
		else
			current_token = current_token->next;
	}
}
void	*expand_tokens(t_memory *memory)
{
	t_tokens	*token;

	token = memory->tokens;
	while (token)
	{
		if (token->type == T_D_QUOTE)
		{
			token->data = expand_double(memory, token->data);
			if (!token->data)
				return (NULL);
		}
		else if (token->type == T_S_QUOTE)
		{
			token->data = expand_single(token->data);
			if (!token->data)
				return (NULL);
		}
		else if (token->type == T_VAR)
		{
			token->data = expand_var(memory, token->data);
			if (!token->data)
				return (NULL);
		}
		// if (token && token->data && ft_strlen(token->data) == 0)
		// 	token->type = T_WHITESPACE;
		token = token->next;
	}
	merge_tokens(memory);
	return (memory);
}

void	print_tokens_as_string(t_memory *memory)
{
	t_tokens	*token;

	token = memory->tokens;
	while (token)
	{
		printf("%s", (char *)token->data);
		token = token->next;
	}
	printf("\n");
}


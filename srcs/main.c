/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/07/29 14:59:19 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_strncpy(char *s, int len)
{
	char *token;
	int i;

	i = 0;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	token[len] = '\0';
	while (i < len)
	{
		token[i] = s[i];
		i++;
	}
	return (token);
}

int print_tokens(t_memory *memory)
{
	t_tokens *current;
	int i;

	i = 1;
	current = memory->tokens;
	printf("Tokens:\n");
	while(current)
	{
		printf("Nr: %d\nToken: %s Type: %d\n", i++, (char*)current->data, current->type);
		current = current->next;
	}
	return (1);
}

int get_type(char *s)
{
	if (*s == D_QUOTE)
		return (T_D_QUOTE);
	else if (*s == S_QUOTE)
		return (T_S_QUOTE);
	else if (*s == DASH)
		return (T_OPTION);
	else
		return (T_WORD);
}

void free_tokens(t_tokens *token)
{
	t_tokens *current;

	current = token;
	while(current)
	{
		current = token->next;
		// free(token->data);
		free(token);
		token = current;
	}
}

int is_whitespace(char *s)
{
	if(*s == ' ' || *s == '\t' || *s == '\n')
		return (1);
	else
		return (0);
}

int is_seperator(char *s)
{
	if(*s == '|' || is_whitespace(s))
		return (1);
	else
		return (0);
}

int skip_whitespace(char *s)
{
	int i;

	i = 0;
	while(*s && is_whitespace(s))
	{
		s++;
		i++;
	}
	return (i);
}

int skip_non_whitespace(char *s)
{
	int i;

	i = 0;
	while(*s && !is_whitespace(s))
	{
		s++;
		i++;
	}
	return (i);
}
void free_memory(t_memory *memory)
{
	if (memory->tokens)
		free_tokens(memory->tokens);
	if (memory->input)
		free(memory->input);
	free(memory);
}

char	*find_seperator(char *s)
{
	while (*s && !is_seperator(s))
		s++;
	return (s);
}

void *process_token(char *s)
{
	void *token;
	int len;

	token = NULL;
	if (*s == '\"')
	{
		if (!ft_strchr((s + 1), '\"'))
			return(NULL);
		len = ft_strchr((s + 1), '\"') - s + 1;
		token = ft_strncpy(s, len);
	}
	else if (*s == '\'')
	{
		if (!ft_strchr((s + 1), '\''))
			return(NULL);
		len = ft_strchr((s + 1), '\'') - s + 1;
		token = ft_strncpy(s, len);
	}
	else if (*s == '-')
	{
		len = find_seperator(s) - s + 1;
		token = ft_strncpy(s, len);
	}
	else if (*s == '|')
	{
		len = find_seperator(s) - s + 1;
		token = ft_strncpy(s, len);
	}
	else
	{
		len = skip_non_whitespace(s);
		token = ft_strncpy(s, len);
	}

	return (token);
}
t_tokens	*create_token(char *s, t_memory *memory)
{
	t_tokens *token;

	token = malloc(sizeof(t_tokens));
	if(!token)
		return (free_memory(memory), NULL);
	token->data = process_token(s);
	if (!token->data)
		return (free_memory(memory), NULL);
	token->type = get_type((char*)token->data);
	token->next = NULL;
	return (token);
}


int	lexer(t_memory *memory)
{
	char *input;
	t_tokens *current = NULL;
	t_tokens *previous = NULL;

	input = memory->input;
	while(*input)
	{
		input += skip_whitespace(input);
		current = create_token(input, memory);
		if (!memory->tokens)
			memory->tokens = current;
		else
        	previous->next = current;
		previous = current;
		input += ft_strlen(current->data);
		input += skip_whitespace(input);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_memory *memory;
	memory = malloc(sizeof(*memory));
	if (!memory)
		return(ERROR);

	(void)ac,
	(void)av;
	memory->input = readline("minishell-> ");
	if (memory->input)
	{
        add_history(memory->input);
		printf("You entered: %s\n", memory->input);
		lexer(memory);
		if (!memory->tokens)
			return(free_memory(memory), ERROR);
		print_tokens(memory);
    }
	return (free_memory(memory), SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/07/28 11:48:38 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int get_type(char *av)
{
	if (*av == R_IN)
		return (T_RET_IN);
	else if (*av == R_OUT)
		return (T_RET_OUT);
	else if (*av == PIPE)
		return (T_PIPE);
	else
		return (T_WORD);
}

void free_tokens(t_tokens *token)
{
	t_tokens *current;
	
	while(current)
	{
		current = token->next;
		free(token->data);
		free(token);
		token = current;
	}
}

void free_memory(t_memory *memory)
{
	if (memory->tokens)
		free_tokens(memory->tokens);
}

t_tokens	*create_token(char *av, t_memory *memory)
{
	t_tokens *token;
	
	token = malloc(sizeof(t_tokens));
	if(!token)
		return (free_memory(memory), NULL);
	token->data = av;
	token->type = get_type(av);
	return (token);
}

int	*lexer(int ac, char **av, t_memory *memory)
{
	t_tokens *current;
	int i;
	
	i = 1;
	memory->tokens = create_token(av[i++], memory);
	current = memory->tokens;
	while(i < ac)
	{
		current->next = create_token(av[i], memory);
		current = current->next;
		if(!current)
			return(NULL);
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_memory *memory;
	memory = malloc(sizeof(memory));
	if (!memory)
		return(ERROR);
	lexer(ac, av, memory);
	if (!memory->tokens)
		return(ERROR);
	return (SUCCESS);
}

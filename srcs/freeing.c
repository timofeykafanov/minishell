/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:04 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/06 15:10:34 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*free_tokens(t_tokens *token)
{
	t_tokens *current;

	current = token;
	while(current)
	{
		current = token->next;
		free(token->data);
		free(token);
		token = current;
	}
	return (NULL);
}

static void free_env(t_env *env)
{
	t_env *current;

	current = env;
	while(current)
	{
		current = env->next;
		free(env->value);
		free(env);
		env = current;
	}
}

void free_memory(t_memory *memory)
{
	if (memory->tokens)
		free_tokens(memory->tokens);
	if (memory->env)
		free_env(memory->env);
	if (memory->input)
		free(memory->input);
	if (memory->path)
		free(memory->path);
	free(memory);
}

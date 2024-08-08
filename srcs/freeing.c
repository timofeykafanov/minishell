/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:04 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/08 10:31:09 by sopperma         ###   ########.fr       */
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

static void free_env(char **env)
{
	int i;

	i = 0;
	while(env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
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
	if (memory->suffix)
		free(memory->suffix);
	free(memory);
}

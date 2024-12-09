/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:04 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/10 16:26:05 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void reset_minishell(t_memory *memory)
{
	free_tokens(memory->tokens);
	memory->tokens = NULL;
	free(memory->suffix);
	memory->suffix = NULL;
	if (memory->faulty_variable_name)
	{
		free(memory->faulty_variable_name);
		memory->faulty_variable_name = NULL;	
	}
	free(memory->heredocs);
	memory->heredocs = NULL;
	if (memory->commands)
		free_commands(memory->commands);
	if (memory->input)
	{
		free(memory->input);
		memory->input = NULL;
	}
	memory->commands = NULL;
}

void	*free_tokens(t_tokens *token)
{
	t_tokens	*current;

	current = token;
	while (current)
	{
		current = token->next;
		if (token->data)
		{
			free(token->data);
			token->data = NULL;	
		}
		free(token);
		token = current;
	}
	return (NULL);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		if (current->args)
		{
			free(current->args);
			current->args = NULL;	
		}
		if (current->path)
		{
			free(current->path);
			current->path = NULL;
		}
		if (current->redir_struct)
		{
			free(current->redir_struct);
			current->redir_struct = NULL;
		}
		free(current);
		current = NULL;
		current = next;
	}
}

void	free_memory(t_memory *memory)
{
	if (memory->heredocs)
		free(memory->heredocs);
	if (memory->tokens)
		free_tokens(memory->tokens);
	if (memory->env)
		free_env(memory->env);
	if (memory->input)
		free(memory->input);
	if (memory->pwd)
		free(memory->pwd);
	if (memory->suffix)
		free(memory->suffix);
	if (memory->faulty_variable_name)
	{
		free(memory->faulty_variable_name);
		memory->faulty_variable_name = NULL;	
	}
	free(memory);
}

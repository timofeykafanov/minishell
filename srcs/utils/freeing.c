/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:04 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/12 18:38:48 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_heredocs(t_memory *memory)
{
	int	i;

	i = 0;
	while(memory->heredocs_count && memory->heredocs[i])
	{
		free(memory->heredocs[i]);
		memory->heredocs[i] = NULL;
		i++;
	}
	memory->heredocs_count = 0;
	free(memory->heredocs);
	memory->heredocs = NULL;
}

void reset_minishell(t_memory *memory)
{
	free_tokens(memory->tokens);
	memory->tokens = NULL;
	if (memory->heredocs)
		free_heredocs(memory);
	free(memory->suffix);
	memory->suffix = NULL;
	if (memory->faulty_variable_name)
	{
		free(memory->faulty_variable_name);
		memory->faulty_variable_name = NULL;	
	}
	if (memory->commands)
		free_commands(memory->commands);
	if (memory->input)
	{
		free(memory->input);
		memory->input = NULL;
	}
	if (memory->pid)
	{
		free(memory->pid);
		memory->pid = NULL;
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
		// printf("freed ending %p %s\n", (void *)token, (char *)token->data);
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

void	free_redir_struct(t_redir_out *current)
{
	t_redir_out	*next;

	while (current)
	{
		next = current->next;
		if (current->heredoc_file_name)
		{
			free(current->heredoc_file_name);
			current->heredoc_file_name = NULL;
		}	
		free(current);
		current = NULL;
		current = next;
	}
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
			free_redir_struct(current->redir_struct);
			current->redir_struct = NULL;
		}
		if (current->env_path)
	{
		free(current->env_path);
		current->env_path = NULL;
	}
		free(current);
		current = NULL;
		current = next;
	}
}

void	free_memory(t_memory *memory)
{
	if (memory->heredocs)
		free_heredocs(memory);
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
	if (memory->commands)
		free_commands(memory->commands);
	if (memory->pid)
	{
		free(memory->pid);
		memory->pid = NULL;
	}
	free(memory);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:33:30 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/27 19:40:26 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_heredocs(t_memory *memory)
{
	int	i;

	i = 0;
	if (memory->heredocs_count != 0)
	{
		while (memory->heredocs && memory->heredocs[i])
		{
			free(memory->heredocs[i]);
			memory->heredocs[i] = NULL;
			i++;
		}
	}
	memory->heredocs_count = 0;
	free(memory->heredocs);
	memory->heredocs = NULL;
}

void	reset_minishell(t_memory *memory)
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

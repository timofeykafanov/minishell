/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:04 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/19 17:14:41 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void end_parser_malloc_error(t_memory *memory, t_parser *p)
{
		free_memory(memory);
		if (p)
			free(p);
		exit(1);
}

void	end_shell(t_memory *memory)
{
	free_memory(memory);
	exit(1);
}

static void	free_command_content(t_command *current)
{
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
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		free_command_content(current);
		free(current);
		current = NULL;
		current = next;
	}
}

static void	free_memory_two(t_memory *memory)
{
	if (memory->commands)
		free_commands(memory->commands);
	if (memory->pid)
	{
		free(memory->pid);
		memory->pid = NULL;
	}
	if (memory->oldpwd)
	{
		free(memory->oldpwd);
		memory->oldpwd = NULL;
	}
	free(memory);
}

void	free_memory(t_memory *memory)
{
	if (memory)
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
		free_memory_two(memory);
	}
}

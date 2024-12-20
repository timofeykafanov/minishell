/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:26:04 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/20 14:34:31 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	end_parser_malloc_error(t_memory *memory, t_parser *p)
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

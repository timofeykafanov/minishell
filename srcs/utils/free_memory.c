/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:28:40 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 14:34:20 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 10:42:54 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];

	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory);
	else
	{
		execute_first_command(command, memory, fd1);
		command = command->next;
		while (command->next)
		{
			execute_next_command(command, memory, fd1);
			command = command->next;
		}
		execute_last_command(command, memory, fd1);
	}
}

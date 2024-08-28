/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:10 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/28 15:17:46 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_command	*create_command(char *name, char **args, int type)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	command->name = name;
	command->args = args;
	command->type = type;
	command->next = NULL;
	return (command);
}

void	parse_command(t_memory *memory)
{
	t_command	*current;
	t_command	*prev_cmd;
	t_tokens	*tokens;
	char		**args;
	int			i;

	tokens = memory->tokens;
	current = NULL;
	memory->commands = NULL;
	while (tokens)
	{
		current = create_command(tokens->data, NULL, tokens->type);
		if (!memory->commands)
			memory->commands = current;
		else
			prev_cmd->next = current;
		i = 0;
		while (tokens && tokens->type != T_PIPE)
		{
			i++;
			if (tokens->next != NULL)
				tokens = tokens->next;
			else
				break ;
		}
		if (tokens->type == T_PIPE)
			tokens = tokens->prev;
		args = (char **)malloc(sizeof(char *) * (i + 1));
		args[i] = NULL;
		while (i >= 0)
		{
			args[i - 1] = (char *)tokens->data;
			i--;
			if (i > 0)
				tokens = tokens->prev;
		}
		current->args = args;
		while (tokens->next != NULL && tokens->type != T_PIPE)
			tokens = tokens->next;
		tokens = tokens->next;
		prev_cmd = current;
	}
}

void print_commands(t_memory *memory)
{
	t_command	*current;
	int			i;

	current = memory->commands;
	while (current)
	{
		i = 0;
		printf("command: %s\n", current->name);
		while (current->args[i])
		{
			printf("arg %d: %s\n", i, current->args[i]);
			i++;
		}
		current = current->next;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:02:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/12 17:05:10 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_commands	*create_command(char *name, int type) {
    t_commands *cmd;

	cmd = (t_commands *)malloc(sizeof(t_commands));
    cmd->name = strdup(name);
    cmd->type = type;
    cmd->next = NULL;
    cmd->child = NULL;
	cmd->main = false;
    return cmd;
}

void	parse_command(t_memory *memory)
{
	t_commands	*root;
	t_commands	*current;
	t_commands	*temp;
	t_commands	*cmd;
	t_tokens	*tokens;

	root = NULL;
	current = NULL;
	temp = NULL;
	tokens = memory->tokens;
	while (tokens)
	{
		if (tokens->type != T_PIPE)
		{
			cmd = create_command(tokens->data, tokens->type);
			if (!root)
			{
				cmd->main = true;
				root = cmd;
				current = cmd;
				current->prev = NULL;
			}
			else
			{
				temp = current;
				current->child = cmd;
				current = cmd;
				current->prev = temp;
			}
		}
		else if (tokens->type == T_PIPE)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == T_WHITESPACE)
				tokens = tokens->next;
			cmd = create_command(tokens->data, tokens->type);
			cmd->main = true;
			while (current->main == false)
				current = current->prev;
			current->next = cmd;
			current = cmd;
			current->prev = temp;
		}
		tokens = tokens->next;
		if (tokens && tokens->type == T_WHITESPACE)
			tokens = tokens->next;
	}
	memory->commands = root;
}

void	print_commands(t_memory *memory)
{
	t_commands	*cmd;
	t_commands	*child;

	cmd = memory->commands;
	while (cmd)
	{
		printf("cmd: %s\n", cmd->name);
		child = cmd->child;
		while (child)
		{
			printf("child: %s\n", child->name);
			child = child->child;
		}
		cmd = cmd->next;
	}
}

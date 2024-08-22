/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 11:02:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/22 15:05:12 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_commands	*create_command(char *name, int type) {
    t_commands *cmd;

	cmd = (t_commands *)malloc(sizeof(t_commands));
    cmd->name = strdup(name);
    cmd->type = type;
    cmd->next = NULL;
    cmd->args = NULL;
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
			if (current && current->type != T_WHITESPACE && \
				tokens->type != T_R_IN && tokens->type != T_R_OUT && \
				tokens->type != T_OUT_APPEND && tokens->type != T_FIND_DELIM)
				current->name = ft_strjoin(current->name, tokens->data);
			else
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
					current->args = cmd;
					current = cmd;
					current->prev = temp;
				}
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
	t_commands	*args;

	cmd = memory->commands;
	while (cmd)
	{
		printf("cmd: %s\n", cmd->name);
		args = cmd->args;
		while (args)
		{
			printf("args: %s\n", args->name);
			args = args->args;
		}
		cmd = cmd->next;
	}
}

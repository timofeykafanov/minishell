/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:22:43 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/18 17:27:08 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_current_token_type(t_parser **p)
{
	if ((*p)->current_token->type == T_WHITESPACE)
	{
		(*p)->current_token = (*p)->current_token->next;
		return (T_WHITESPACE);
	}
	if ((*p)->current_token->type == T_R_OUT \
		|| (*p)->current_token->type == T_OUT_APPEND
		|| (*p)->current_token->type == T_R_IN \
		|| (*p)->current_token->type == T_HEREDOC)
		return (T_R_IN);
	return (0);
}

t_command	*create_command(char *name, char **args, int type)
{
	t_command	*command;

	command = (t_command *)malloc(sizeof(t_command));
	command->name = name;
	command->args = args;
	command->type = type;
	command->redir_struct = NULL;
	command->next = NULL;
	command->path = NULL;
	command->env_path = NULL;
	command->is_filename = false;
	return (command);
}

t_parser	*init_parser(t_memory *memory)
{
	t_parser	*parser;

	parser = (t_parser *)malloc(sizeof(t_parser));
	memory->commands = NULL;
	parser->current_cmd = NULL;
	parser->prev_cmd = NULL;
	parser->start_token = memory->tokens;
	parser->current_token = parser->start_token;
	parser->current_redir = NULL;
	parser->last_redir = NULL;
	parser->args_count = 0;
	parser->heredoc_count = 0;
	return (parser);
}

void	parser_init_phase_two(t_parser **p, t_memory *memory)
{
	(*p)->current_token = (*p)->start_token;
	(*p)->current_cmd->args = (char **)malloc(sizeof(char *) \
		* ((*p)->args_count + 1));
	(*p)->current_cmd->args[(*p)->args_count] = NULL;
	memory->heredocs_count += (*p)->heredoc_count;
	(*p)->args_count = 0;
	(*p)->heredoc_count = 0;
}

void	print_commands(t_memory *memory)
{
	t_command	*current;
	int			i;
	t_redir_out	*redir;

	current = memory->commands;
	while (current)
	{
		redir = current->redir_struct;
		i = 0;
		printf("command: %s\n", current->name);
		while (current->args[i])
		{
			printf("arg %d: %s\n", i, current->args[i]);
			i++;
		}
		i = 0;
		while (redir)
		{
			printf("red %d: %s type: %d quoted: %d \
				heredoc_filename: %s\n", i, redir->file_name, \
				redir->type, redir->was_quoted, redir->heredoc_file_name);
			redir = redir->next;
			i++;
		}
		current = current->next;
	}
}

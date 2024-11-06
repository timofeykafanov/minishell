/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:10 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 17:15:29 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	return (command);
}

static t_parser	*init_parser(t_memory *memory)
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

void parser_phase_one(t_parser **p, t_memory *memory)
{
	(*p)->current_cmd = create_command((*p)->current_token->data, NULL, (*p)->current_token->type);
	if (!memory->commands)
		memory->commands = (*p)->current_cmd;
	else
		(*p)->prev_cmd->next = (*p)->current_cmd;
	(*p)->args_count = 0;
	while ((*p)->current_token)
	{
		if((*p)->current_token->type == T_WHITESPACE)
		{
			(*p)->current_token = (*p)->current_token->next;
			continue;
		}
		if(((*p)->current_token->type == T_R_OUT || (*p)->current_token->type == T_OUT_APPEND
			|| (*p)->current_token->type == T_R_IN || (*p)->current_token->type == T_HEREDOC ) && (*p)->current_token->next != NULL)
		{
			if ((*p)->current_token->type == T_HEREDOC)
				(*p)->heredoc_count++;
			if((*p)->current_token->next->type == T_WHITESPACE && (*p)->current_token->next->next != NULL)
				(*p)->current_token = (*p)->current_token->next->next->next;
			else
				(*p)->current_token = (*p)->current_token->next->next;
			continue;
		}
		if ((*p)->current_token->type == T_PIPE)
			break;
		(*p)->args_count++;
		(*p)->current_token = (*p)->current_token->next;
	}
}
static void parser_init_phase_two(t_parser **p, t_memory *memory)
{
	(*p)->current_token = (*p)->start_token;
	(*p)->current_cmd->args = (char **)malloc(sizeof(char *) * ((*p)->args_count + 1));
	(*p)->current_cmd->args[(*p)->args_count] = NULL;
	memory->heredocs = (char **)malloc(sizeof(char *) * ((*p)->heredoc_count + 1));
	memory->heredocs [(*p)->heredoc_count] = NULL;
	memory->heredocs_count = (*p)->heredoc_count;
	(*p)->args_count = 0;
	(*p)->heredoc_count = 0;
}
static void parser_phase_two(t_parser **p)
{
	while ((*p)->current_token)
		{
			if((*p)->current_token->type == T_WHITESPACE)
			{
				(*p)->current_token = (*p)->current_token->next;
				continue;
			}
			if(((*p)->current_token->type == T_R_OUT || (*p)->current_token->type == T_OUT_APPEND \
			|| (*p)->current_token->type == T_R_IN || (*p)->current_token->type == T_HEREDOC) && (*p)->current_token->next != NULL)
			{
				if((*p)->current_token->next->type == T_WHITESPACE && (*p)->current_token->next->next != NULL)
					(*p)->current_token = (*p)->current_token->next;
				(*p)->current_redir = malloc(sizeof(t_redir_out));
				(*p)->current_redir->file_name = (*p)->current_token->next->data;
				(*p)->current_redir->was_quoted = (*p)->current_token->next->was_quoted;
				if ((*p)->current_token->type == T_WHITESPACE)
					(*p)->current_redir->type = (*p)->current_token->prev->type;
				else
					(*p)->current_redir->type = (*p)->current_token->type;
				// if (current_redir->type == T_HEREDOC)
				// {
				// 	memory->heredocs[heredoc_count] = current_redir->file_name;
				// 	heredoc_count++;
				// }
				(*p)->current_redir->next = NULL;
				if(!(*p)->current_cmd->redir_struct)
				{
					(*p)->current_cmd->redir_struct = (*p)->current_redir;
					(*p)->last_redir = (*p)->current_redir;	
				}
				else
				{
					(*p)->last_redir->next = (*p)->current_redir;
					(*p)->last_redir = (*p)->current_redir;
				}
				(*p)->current_token = (*p)->current_token->next->next;
				continue;
			}
			if ((*p)->current_token->type == T_PIPE)
			{
				(*p)->current_token = (*p)->current_token->next;
				break;
			}	
			(*p)->current_cmd->args[(*p)->args_count] = (*p)->current_token->data;	
			(*p)->current_token = (*p)->current_token->next;
			(*p)->args_count++;
		}
}
void	parse_command(t_memory *memory)
{
	t_parser	*parser;

	parser = init_parser(memory);
	while (parser->current_token)
	{
		if(parser->current_token->type == T_WHITESPACE)
		{
			parser->current_token = parser->current_token->next;
			continue;
		}
		// current_cmd = create_command(current_token->data, NULL, current_token->type);
		// if (!memory->commands)
		// 	memory->commands = current_cmd;
		// else
		// 	prev_cmd->next = current_cmd;
		// args_count = 0;
		// while (current_token)
		// {
		// 	if(current_token->type == T_WHITESPACE)
		// 	{
		// 		current_token = current_token->next;
		// 		continue;
		// 	}
		// 	if((current_token->type == T_R_OUT || current_token->type == T_OUT_APPEND
		// 		|| current_token->type == T_R_IN || current_token->type == T_HEREDOC ) && current_token->next != NULL)
		// 	{
		// 		if (current_token->type == T_HEREDOC)
		// 			heredoc_count++;
		// 		if(current_token->next->type == T_WHITESPACE && current_token->next->next != NULL)
		// 			current_token = current_token->next->next->next;
		// 		else
		// 			current_token = current_token->next->next;
		// 		continue;
		// 	}
		// 	if (current_token->type == T_PIPE)
		// 		break;
		// 	args_count++;
		// 	current_token = current_token->next;
		// }
		parser_phase_one(&parser, memory);
		// current_token = start_token;
		// current_cmd->args = (char **)malloc(sizeof(char *) * (args_count + 1));
		// current_cmd->args[args_count] = NULL;
		// memory->heredocs = (char **)malloc(sizeof(char *) * (heredoc_count + 1));
		// memory->heredocs [heredoc_count] = NULL;
		// memory->heredocs_count = heredoc_count;
		// args_count = 0;
		// heredoc_count = 0;
		parser_init_phase_two(&parser, memory);
		// while (current_token)
		// {
		// 	if(current_token->type == T_WHITESPACE)
		// 	{
		// 		current_token = current_token->next;
		// 		continue;
		// 	}
		// 	if((current_token->type == T_R_OUT || current_token->type == T_OUT_APPEND \
		// 	|| current_token->type == T_R_IN || current_token->type == T_HEREDOC) && current_token->next != NULL)
		// 	{
		// 		if(current_token->next->type == T_WHITESPACE && current_token->next->next != NULL)
		// 			current_token = current_token->next;
		// 		current_redir = malloc(sizeof(t_redir_out));
		// 		current_redir->file_name = current_token->next->data;
		// 		current_redir->was_quoted = current_token->next->was_quoted;
		// 		if (current_token->type == T_WHITESPACE)
		// 			current_redir->type = current_token->prev->type;
		// 		else
		// 			current_redir->type = current_token->type;
		// 		// if (current_redir->type == T_HEREDOC)
		// 		// {
		// 		// 	memory->heredocs[heredoc_count] = current_redir->file_name;
		// 		// 	heredoc_count++;
		// 		// }
		// 		current_redir->next = NULL;
		// 		if(!current_cmd->redir_struct)
		// 		{
		// 			current_cmd->redir_struct = current_redir;
		// 			last_redir = current_redir;	
		// 		}
		// 		else
		// 		{
		// 			last_redir->next = current_redir;
		// 			last_redir = current_redir;
		// 		}
		// 		current_token = current_token->next->next;
		// 		continue;
		// 	}
		// 	if (current_token->type == T_PIPE)
		// 	{
		// 		current_token = current_token->next;
		// 		break;
		// 	}	
		// 	current_cmd->args[args_count] = current_token->data;	
		// 	current_token = current_token->next;
		// 	args_count++;
		// }
		parser_phase_two(&parser);
		parser->start_token = parser->current_token;
		parser->prev_cmd = parser->current_cmd;
	}
}

// void print_commands(t_memory *memory)
// {
// 	t_command	*current;
// 	int			i;
// 	t_redir_out	*redir;

// 	current = memory->commands;
// 	while (current)
// 	{
// 		redir = current->redir_struct;
// 		i = 0;
// 		printf("command: %s\n", current->name);
// 		while (current->args[i])
// 		{
// 			printf("arg %d: %s\n", i, current->args[i]);
// 			i++;
// 		}
// 		i = 0;
// 		// while (current->redir_out[i])
// 		// {
// 		// 	printf("red %d: %s\n", i, current->redir_out[i]);
// 		// 	i++;
// 		// }
// 		while (redir)
// 		{
// 			printf("red %d: %s type: %d quoted: %d heredoc_filename: %s\n", i, redir->file_name, redir->type, redir->was_quoted, redir->heredoc_file_name);
// 			redir = redir->next;
// 			i++;
// 		}
// 		current = current->next;
// 	}
// }

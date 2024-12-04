/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:10 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/04 17:03:43 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_command	*create_command(char *name, char **args, int type)
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

static int check_current_token_type(t_parser **p)
{
	if ((*p)->current_token->type == T_WHITESPACE)
	{
		(*p)->current_token = (*p)->current_token->next;
		return (T_WHITESPACE);
	}
	if ((*p)->current_token->type == T_R_OUT || (*p)->current_token->type == T_OUT_APPEND
			|| (*p)->current_token->type == T_R_IN || (*p)->current_token->type == T_HEREDOC)
		return (T_R_IN);
	return (0);
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
		if (check_current_token_type(p) == T_WHITESPACE)
			continue;
		if ((check_current_token_type(p) == T_R_IN) && (*p)->current_token->next != NULL)
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

static void handle_redirect_parser(t_parser *p)
{
	if(p->current_token->next->type == T_WHITESPACE && p->current_token->next->next != NULL)
		p->current_token = p->current_token->next;
	p->current_redir = malloc(sizeof(t_redir_out));
	p->current_redir->was_quoted = p->current_token->next->was_quoted;
	p->current_redir->file_name = p->current_token->next->data;
	if (p->current_token->type == T_WHITESPACE)
		p->current_redir->type = p->current_token->prev->type;
	else
		p->current_redir->type = p->current_token->type;
	p->current_redir->next = NULL;
	if(!p->current_cmd->redir_struct)
	{
		p->current_cmd->redir_struct = p->current_redir;
		p->last_redir = p->current_redir;	
	}
	else
	{
		p->last_redir->next = p->current_redir;
		p->last_redir = p->current_redir;
	}
	p->current_token = p->current_token->next->next;
}

static void parser_phase_two(t_parser *p)
{
	while (p->current_token)
	{
		if (check_current_token_type(&p) == T_WHITESPACE)
			continue;
		if ((check_current_token_type(&p) == T_R_IN) && p->current_token->next != NULL)
		{
			handle_redirect_parser(p);
			continue;
		}
		if (p->current_token->type == T_PIPE)
		{
			p->current_token = p->current_token->next;
			break;
		}	
		p->current_cmd->args[p->args_count] = p->current_token->data;	
		p->current_token = p->current_token->next;
		p->args_count++;
	}
}

void	parse_command(t_memory *memory)
{
	t_parser	*p;

	p = init_parser(memory);
	while (p->current_token)
	{
		parser_phase_one(&p, memory);
		parser_init_phase_two(&p, memory);
		parser_phase_two(p);
		p->start_token = p->current_token;
		p->prev_cmd = p->current_cmd;
	}
	free(p);
}

void print_commands(t_memory *memory)
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
		// while (current->redir_out[i])
		// {
		// 	printf("red %d: %s\n", i, current->redir_out[i]);
		// 	i++;
		// }
		while (redir)
		{
			printf("red %d: %s type: %d quoted: %d heredoc_filename: %s\n", i, redir->file_name, redir->type, redir->was_quoted, redir->heredoc_file_name);
			redir = redir->next;
			i++;
		}
		current = current->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:10 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/18 17:24:42 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parser_phase_one_loop(t_parser *p)
{
	while (p->current_token)
	{
		if (check_current_token_type(&p) == T_WHITESPACE)
			continue ;
		if (p->current_token->type == T_HEREDOC)
			p->heredoc_count = p->heredoc_count + 1;
		if ((check_current_token_type(&p) == T_R_IN) \
			&& p->current_token->next != NULL)
		{
			if (p->current_token->next->type == T_WHITESPACE \
				&& p->current_token->next->next != NULL)
				p->current_token = p->current_token->next->next->next;
			else
				p->current_token = p->current_token->next->next;
			continue ;
		}
		if (p->current_token->type == T_PIPE)
			break ;
		p->args_count++;
		p->current_token = p->current_token->next;
	}
}

void	parser_phase_one(t_parser *p, t_memory *memory)
{
	if (is_redirect(p->current_token->type))
		p->current_cmd = create_command("", NULL, p->current_token->type);
	else
		p->current_cmd = create_command(p->current_token->data, \
			NULL, p->current_token->type);
	if (!memory->commands)
		memory->commands = p->current_cmd;
	else
		p->prev_cmd->next = p->current_cmd;
	p->args_count = 0;
	parser_phase_one_loop(p);
}

static void	handle_redirect_parser(t_parser *p)
{
	if (p->current_token->next->type == T_WHITESPACE \
		&& p->current_token->next->next != NULL)
		p->current_token = p->current_token->next;
	p->current_redir = malloc(sizeof(t_redir_out));
	p->current_redir->heredoc_file_name = NULL;
	p->current_redir->was_quoted = p->current_token->next->was_quoted;
	p->current_redir->file_name = p->current_token->next->data;
	if (p->current_token->type == T_WHITESPACE)
		p->current_redir->type = p->current_token->prev->type;
	else
		p->current_redir->type = p->current_token->type;
	p->current_redir->next = NULL;
	if (!p->current_cmd->redir_struct)
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

static void	parser_phase_two(t_parser *p)
{
	while (p->current_token)
	{
		if (check_current_token_type(&p) == T_WHITESPACE)
			continue ;
		if ((check_current_token_type(&p) == T_R_IN) \
			&& p->current_token->next != NULL)
		{
			handle_redirect_parser(p);
			continue ;
		}
		if (p->current_token->type == T_PIPE)
		{
			p->current_token = p->current_token->next;
			break ;
		}
		p->current_cmd->args[p->args_count] = p->current_token->data;
		p->current_token = p->current_token->next;
		p->args_count++;
	}
	if (p->current_cmd->args && p->current_cmd->name \
		&& ft_strlen(p->current_cmd->name) == 0)
		p->current_cmd->name = p->current_cmd->args[0];
}

void	parse_command(t_memory *memory)
{
	t_parser	*p;

	p = init_parser(memory);
	while (p->current_token)
	{
		parser_phase_one(p, memory);
		parser_init_phase_two(&p, memory);
		parser_phase_two(p);
		p->start_token = p->current_token;
		p->prev_cmd = p->current_cmd;
	}
	memory->heredocs = (char **)malloc(sizeof(char *) \
		* (memory->heredocs_count + 1));
	memory->heredocs [memory->heredocs_count] = NULL;
	free(p);
}

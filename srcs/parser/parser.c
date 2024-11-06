/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:10 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 15:56:21 by sopperma         ###   ########.fr       */
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

void	parse_command(t_memory *memory)
{
	t_command	*current_cmd;
	t_command	*prev_cmd;
	t_tokens	*start_token;
	t_tokens	*current_token;
	t_redir_out	*current_redir;
	t_redir_out	*last_redir;
	int			args_count;
	int 		heredoc_count;

	memory->commands = NULL;
	start_token = memory->tokens;
	current_token = start_token;
	current_cmd = NULL;
	heredoc_count = 0;
	while (current_token)
	{
		if(current_token->type == T_WHITESPACE)
		{
			current_token = current_token->next;
			continue;
		}
		current_cmd = create_command(current_token->data, NULL, current_token->type);
		if (!memory->commands)
			memory->commands = current_cmd;
		else
			prev_cmd->next = current_cmd;
		args_count = 0;
		while (current_token)
		{
			if(current_token->type == T_WHITESPACE)
			{
				current_token = current_token->next;
				continue;
			}
			if((current_token->type == T_R_OUT || current_token->type == T_OUT_APPEND
				|| current_token->type == T_R_IN || current_token->type == T_HEREDOC ) && current_token->next != NULL)
			{
				if (current_token->type == T_HEREDOC)
					heredoc_count++;
				if(current_token->next->type == T_WHITESPACE && current_token->next->next != NULL)
					current_token = current_token->next->next->next;
				else
					current_token = current_token->next->next;
				continue;
			}
			if (current_token->type == T_PIPE)
				break;
			args_count++;
			current_token = current_token->next;
		}
		current_token = start_token;
		current_cmd->args = (char **)malloc(sizeof(char *) * (args_count + 1));
		current_cmd->args[args_count] = NULL;
		memory->heredocs = (char **)malloc(sizeof(char *) * (heredoc_count + 1));
		memory->heredocs [heredoc_count] = NULL;
		memory->heredocs_count = heredoc_count;
		args_count = 0;
		heredoc_count = 0;
		while (current_token)
		{
			if(current_token->type == T_WHITESPACE)
			{
				current_token = current_token->next;
				continue;
			}
			if((current_token->type == T_R_OUT || current_token->type == T_OUT_APPEND \
			|| current_token->type == T_R_IN || current_token->type == T_HEREDOC) && current_token->next != NULL)
			{
				if(current_token->next->type == T_WHITESPACE && current_token->next->next != NULL)
					current_token = current_token->next;
				current_redir = malloc(sizeof(t_redir_out));
				current_redir->file_name = current_token->next->data;
				current_redir->was_quoted = current_token->next->was_quoted;
				if (current_token->type == T_WHITESPACE)
					current_redir->type = current_token->prev->type;
				else
					current_redir->type = current_token->type;
				// if (current_redir->type == T_HEREDOC)
				// {
				// 	memory->heredocs[heredoc_count] = current_redir->file_name;
				// 	heredoc_count++;
				// }
				current_redir->next = NULL;
				if(!current_cmd->redir_struct)
				{
					current_cmd->redir_struct = current_redir;
					last_redir = current_redir;	
				}
				else
				{
					last_redir->next = current_redir;
					last_redir = current_redir;
				}
				current_token = current_token->next->next;
				continue;
			}
			if (current_token->type == T_PIPE)
			{
				current_token = current_token->next;
				break;
			}	
			current_cmd->args[args_count] = current_token->data;	
			current_token = current_token->next;
			args_count++;
		}
		start_token = current_token;
		prev_cmd = current_cmd;
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

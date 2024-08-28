/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:10 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/28 15:59:55 by sopperma         ###   ########.fr       */
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
	t_command	*current_cmd;
	t_command	*prev_cmd;
	t_tokens	*start_token;
	t_tokens	*current_token;
	int			args_count;
	int 		red_out_count;

	memory->commands = NULL;
	start_token = memory->tokens;
	current_token = start_token;
	current_cmd = NULL;
	while (current_token)
	{
		current_cmd = create_command(current_token->data, NULL, current_token->type);
		if (!memory->commands)
			memory->commands = current_cmd;
		else
			prev_cmd->next = current_cmd;
		args_count = 0;
		red_out_count = 0;
		while (current_token)
		{
			if(current_token->type == T_R_OUT && current_token->next != NULL)
			{
				red_out_count++;
				current_token = current_token->next->next;
				continue;
			}
			if (current_token->type == T_PIPE)
			{
				break;
			}	
			args_count++;
			current_token = current_token->next;
		}
		current_token = start_token;
		current_cmd->args = (char **)malloc(sizeof(char *) * (args_count + 1));
		current_cmd->args[args_count] = NULL;
		current_cmd->redir_out = (char **)malloc(sizeof(char *) * (red_out_count + 1));
		current_cmd->redir_out[red_out_count] = NULL;
		args_count = 0;
		red_out_count = 0;
		while (current_token)
		{
			if(current_token->type == T_R_OUT && current_token->next != NULL)
			{
				current_cmd->redir_out[red_out_count] = current_token->next->data;
				current_token = current_token->next->next;
				red_out_count++;
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

// void	parse_command(t_memory *memory)
// {
// 	t_command	*current;
// 	t_command	*prev_cmd;
// 	t_tokens	*tokens;
// 	char		**args;
// 	int			i;

// 	tokens = memory->tokens;
// 	current = NULL;
// 	while (tokens)
// 	{
// 		current = create_command(tokens->data, NULL, tokens->type);
// 		if (!memory->commands)
// 			memory->commands = current;
// 		else
// 			prev_cmd->next = current;
// 		i = 0;
// 		while (tokens && tokens->type != T_PIPE)
// 		{
// 			i++;
// 			if (tokens->next != NULL)
// 				tokens = tokens->next;
// 			else
// 				break ;
// 		}
// 		if (tokens->type == T_PIPE)
// 			tokens = tokens->prev;
// 		args = (char **)malloc(sizeof(char *) * (i + 1));
// 		args[i] = NULL;
// 		while (i >= 0)
// 		{
// 			args[i - 1] = (char *)tokens->data;
// 			i--;
// 			if (i > 0)
// 				tokens = tokens->prev;
// 		}
// 		current->args = args;
// 		while (tokens->next != NULL && tokens->type != T_PIPE)
// 			tokens = tokens->next;
// 		tokens = tokens->next;
// 		prev_cmd = current;
// 	}
// }

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
		i = 0;
		while (current->redir_out[i])
		{
			printf("red %d: %s\n", i, current->redir_out[i]);
			i++;
		}
		current = current->next;
	}
}
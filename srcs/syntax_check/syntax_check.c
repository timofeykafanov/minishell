/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:40:48 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 14:50:12 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	show_error_message(t_tokens *token)
{
	if (token->next->type == T_R_OUT)
		ft_printf("syntax error near unexpected token `>'\n", \
			STDERR_FILENO);
	else if (token->next->type == T_R_IN)
		ft_printf("syntax error near unexpected token `<'\n", \
			STDERR_FILENO);
	else if (token->next->type == T_OUT_APPEND)
		ft_printf("syntax error near unexpected token `>>'\n", \
			STDERR_FILENO);
	else if (token->next->type == T_HEREDOC)
		ft_printf("syntax error near unexpected token `<<'\n", \
			STDERR_FILENO);
	else if (token->next->type == T_PIPE)
		ft_printf("syntax error near unexpected token `|'\n", \
			STDERR_FILENO);
}

static int	check_first_token(t_tokens *token, t_memory *memory)
{
	if (token && ft_strncmp(token->data, ".", 1) == 0
		&& ft_strlen(token->data) == 1)
	{
		ft_printf(".: filename argument required\n", STDERR_FILENO);
		memory->exit_status = 2;
		return (1);
	}
	if (token && token->type == T_PIPE)
	{
		ft_printf("syntax error near unexpected token `|'\n", STDERR_FILENO);
		memory->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	check_after_pipe(t_tokens *token, t_memory *memory)
{
	if (token->next && token->next->type == T_WHITESPACE)
		token = token->next;
	if (token->next && ft_strncmp(token->next->data, ".", 1) == 0
		&& ft_strlen(token->next->data) == 1)
	{
		ft_printf(".: filename argument required\n", STDERR_FILENO);
		memory->exit_status = 2;
		return (1);
	}
	if (!token->next || token->next->type == T_PIPE)
	{
		ft_printf("syntax error near unexpected token `|'\n", \
			STDERR_FILENO);
		memory->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	check_after_redir(t_tokens *token, t_memory *memory)
{
	if (token->next && token->next->type == T_WHITESPACE)
		token = token->next;
	if (!token->next)
	{
		ft_printf("syntax error near unexpected token `newline'\n", \
			STDERR_FILENO);
		memory->exit_status = 2;
		return (1);
	}
	else if (token->next->type == T_R_OUT
		|| token->next->type == T_R_IN
		|| token->next->type == T_OUT_APPEND
		|| token->next->type == T_HEREDOC
		|| token->next->type == T_PIPE)
	{
		show_error_message(token);
		memory->exit_status = 2;
		return (1);
	}
	return (0);
}

int	syntax_check(t_memory *memory)
{
	t_tokens	*token;

	token = memory->tokens;
	if (check_first_token(token, memory) == 1)
		return (1);
	while (token)
	{
		if (token->type == T_PIPE)
		{
			if (check_after_pipe(token, memory) == 1)
				return (1);
		}
		else if (token->type == T_R_OUT || token->type == T_R_IN
			|| token->type == T_OUT_APPEND || token->type == T_HEREDOC
			|| token->type == T_PIPE)
		{
			if (check_after_redir(token, memory) == 1)
				return (1);
		}
		token = token->next;
	}
	return (0);
}

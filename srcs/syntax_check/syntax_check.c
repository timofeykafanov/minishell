/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:40:48 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/19 14:26:25 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_check(t_memory *memory)
{
	t_tokens	*token;

	token = memory->tokens;
	if (token && ft_strncmp(token->data, ".", ft_strlen(token->data)) == 0)
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
	while (token)
	{
		if (token->type == T_PIPE)
		{
			if (token->next && token->next->type == T_WHITESPACE)
				token = token->next;
			if (token->next && ft_strncmp(token->next->data, ".", ft_strlen(token->next->data)) == 0)
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
		}
		else if (token->type == T_R_OUT || token->type == T_R_IN
			|| token->type == T_OUT_APPEND || token->type == T_HEREDOC
			|| token->type == T_PIPE)
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
				memory->exit_status = 2;
				return (1);
			}
		}
		token = token->next;
	}
	return (0);
}

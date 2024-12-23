/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:55:23 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 15:48:16 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_redir_out(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
			redir_out(redir, mem, has_child);
		else if (redir->type == T_OUT_APPEND)
			redir_append(redir, mem, has_child);
		redir = redir->next;
	}
}

void	handle_redir_in(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_IN)
			redir_in(redir, mem, has_child);
		else if (redir->type == T_HEREDOC)
			redir_heredoc(redir, mem, has_child);
		redir = redir->next;
	}
}

void	handle_redir(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
			redir_out(redir, mem, has_child);
		else if (redir->type == T_OUT_APPEND)
			redir_append(redir, mem, has_child);
		else if (redir->type == T_R_IN)
			redir_in(redir, mem, has_child);
		else if (redir->type == T_HEREDOC)
			redir_heredoc(redir, mem, has_child);
		redir = redir->next;
	}
}

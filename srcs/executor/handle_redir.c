/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:55:23 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/08 17:32:47 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redir_out(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;
	int			res;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
			res = redir_out(redir, mem, has_child);
		else if (redir->type == T_OUT_APPEND)
			res = redir_append(redir, mem, has_child);
		if (res == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (SUCCESS);
}

int	handle_redir_in(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;
	int			res;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_IN)
			res = redir_in(cmd, redir, mem, has_child);
		else if (redir->type == T_HEREDOC)
			res = redir_heredoc(cmd, redir, mem, has_child);
		if (res == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (SUCCESS);
}

int	handle_redir(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;
	int			res;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
			res = redir_out(redir, mem, has_child);
		else if (redir->type == T_OUT_APPEND)
			res = redir_append(redir, mem, has_child);
		else if (redir->type == T_R_IN)
			res = redir_in(cmd, redir, mem, has_child);
		else if (redir->type == T_HEREDOC)
			res = redir_heredoc(cmd, redir, mem, has_child);
		if (res == ERROR)
			return (ERROR);
		redir = redir->next;
	}
	return (SUCCESS);
}

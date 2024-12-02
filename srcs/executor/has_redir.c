/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:43:56 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/02 16:44:05 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_redir_in(t_command *cmd)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_IN || redir->type == T_HEREDOC)
			return (true);
		redir = redir->next;
	}
	return (false);
}

bool	has_redir_out(t_command *cmd)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT || redir->type == T_OUT_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}
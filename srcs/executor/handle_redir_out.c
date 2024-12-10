/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:26:44 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/10 16:28:58 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	redir_out(t_redir_out *redir)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_out == -1)
	{
		ft_printf("%s: ", STDERR_FILENO, redir->file_name);
		perror("");
		exit(1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
}

static void	redir_append(t_redir_out *redir)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		ft_printf("%s: ", STDERR_FILENO, redir->file_name);
		perror("");
		exit(1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
}

void	handle_redir_out(t_command *cmd)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
			redir_out(redir);
		if (redir->type == T_OUT_APPEND)
			redir_append(redir);
		redir = redir->next;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:26:44 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/28 14:58:16 by tkafanov         ###   ########.fr       */
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

// static void	redir_in(t_redir_out *redir)
// {
// 	int	fd_in;

// 	fd_in = open(redir->file_name, O_RDONLY);
// 	if (fd_in == -1)
// 	{
// 		ft_printf("%s: ", STDERR_FILENO, redir->file_name);
// 		perror("");
// 		exit(1);
// 	}
// 	printf("Before dup2: fd_in = %d\n", fd_in);
// 	dup2(fd_in, STDIN_FILENO);
// 	printf("After dup2: STDIN_FILENO = %d\n", STDIN_FILENO);
// 	close(fd_in);
// }

// static void redir_heredoc(t_redir_out *redir)
// {
// 	int	fd_heredoc;
	
// 	fd_heredoc = open(redir->heredoc_file_name, O_RDONLY);
// 	if (fd_heredoc == -1)
// 	{
// 		ft_printf("%s: ", STDERR_FILENO, redir->heredoc_file_name);
// 		perror("");
// 		exit(1);
// 	}
// 	dup2(fd_heredoc, STDIN_FILENO);
// 	close(fd_heredoc);
// }

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
		// if (redir->type == T_R_IN)
		// 	redir_in(redir);
		// if (redir->type == T_HEREDOC)
		// 	redir_heredoc(redir);
		redir = redir->next;
	}
}

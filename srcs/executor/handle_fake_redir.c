/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fake_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:35:02 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 15:32:45 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../includes/minishell.h"

// static void	fake_redir_out(t_redir_out *redir)
// {
// 	int	fd_out;

// 	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
// 	if (fd_out == -1)
// 	{
// 		ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
// 		perror("");
// 		exit(1);
// 	}
// 	close(fd_out);
// }

// static void	fake_redir_append(t_redir_out *redir)
// {
// 	int	fd_out;

// 	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd_out == -1)
// 	{
// 		ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
// 		perror("");
// 		exit(1);
// 	}
// 	close(fd_out);
// }

// static void	fake_redir_in(t_redir_out *redir)
// {
// 	int	fd_in;

// 	fd_in = open(redir->file_name, O_RDONLY);
// 	if (fd_in == -1)
// 	{
// 		ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
// 		perror("");
// 		exit(1);
// 	}
// 	close(fd_in);
// }

// void	handle_fake_redir(t_command *cmd)
// {
// 	t_redir_out	*redir;

// 	redir = cmd->redir_struct;
// 	while (redir)
// 	{
// 		if (redir->type == T_R_OUT)
// 			fake_redir_out(redir);
// 		if (redir->type == T_OUT_APPEND)
// 			fake_redir_append(redir);
// 		if (redir->type == T_R_IN)
// 			fake_redir_in(redir);
// 		redir = redir->next;
// 	}
// }

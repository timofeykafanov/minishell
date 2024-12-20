/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:51 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 21:01:46 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern sig_atomic_t	g_signal;

void	handle_sigint(int sig)
{
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal = sig;
}

void	handle_heredoc_sigint(int sig)
{
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal = sig;
}

void	handle_child_sigint(int sig)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal = sig;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	set_signals(int type)
{
	if (type == MAIN)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == CHILD)
	{
		signal(SIGINT, handle_child_sigint);
		signal(SIGQUIT, handle_sigquit);
	}
	else if (type == HEREDOC)
	{
		signal(SIGINT, handle_heredoc_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == WAIT)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, handle_sigquit);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:51 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 13:46:01 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern sig_atomic_t g_signal;

// static void	handle_sigint(int sig)
// {
// 	(void)sig;
// 	// ft_printf("g_signal in handler: %d\n", g_signal);
// 	ft_printf("\n", STDIN_FILENO);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	g_signal = 1;
// }

// // static void	handle_child_sigint(int sig)
// // {
// // 	(void)sig;
// // 	g_signal = 1;
// // 	ft_printf("\n", STDOUT_FILENO);
// // 	// exit(130);
// // }

// static void	handle_sigquit(int sig)
// {
// 	(void)sig;
// 	g_signal = 2;
// 	ioctl(STDIN_FILENO, TIOCSTI, "Quit (core dumped)\n");
// 	// exit(131);
// }

// static void	handle_heredoc_sigint(int sig)
// {
// 	(void)sig;
// 	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	// rl_on_new_line();
// 	// rl_replace_line("", 0);
// 	g_signal = 1;
// }



void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		// rl_redisplay();
		g_signal = sig;
	}
}

void	handle_heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		// rl_redisplay();
		g_signal = sig;
	}
}

void	handle_child_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		g_signal = sig;
	}
}

void	handle_sigquit(int sig)
{
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
	}
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

// void handle_sigint(int sig)
// {
//     if (sig == SIGINT)
//     {
//         ft_printf("\n", STDOUT_FILENO);
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         rl_redisplay();
//         g_signal = sig;
//     }
// }

// void handle_child_sigint(int sig)
// {
//     if (sig == SIGINT)
//     {
//         ft_printf("\n", STDOUT_FILENO);
//         g_signal = sig;
//         exit(130);  // Standard SIGINT exit code
//     }
// }

// void handle_child_sigquit(int sig)
// {
//     if (sig == SIGQUIT)
//     {
//         ft_printf("Quit (core dumped)\n", STDOUT_FILENO);
//         g_signal = sig;
//         exit(131);  // Standard SIGQUIT exit code
//     }
// }

// void handle_heredoc_sigint(int sig)
// {
//     if (sig == SIGINT)
//     {
//         ft_printf("\n", STDOUT_FILENO);
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         g_signal = sig;
//         exit(130);
//     }
// }

// void set_signals(int type)
// {
//     struct sigaction sa_int;
//     struct sigaction sa_quit;
    
//     sigemptyset(&sa_int.sa_mask);
//     sigemptyset(&sa_quit.sa_mask);
//     sa_int.sa_flags = 0;
//     sa_quit.sa_flags = 0;
    
//     if (type == MAIN)
//     {
//         sa_int.sa_handler = handle_sigint;
//         sa_quit.sa_handler = SIG_IGN;
//     }
//     else if (type == CHILD)
//     {
//         sa_int.sa_handler = handle_child_sigint;
//         sa_quit.sa_handler = handle_child_sigquit;
//     }
//     else if (type == HEREDOC)
//     {
//         sa_int.sa_handler = handle_heredoc_sigint;
//         sa_quit.sa_handler = SIG_IGN;
//     }
//     else if (type == WAIT)
//     {
//         sa_int.sa_handler = SIG_IGN;
//         sa_quit.sa_handler = SIG_IGN;
//     }
    
//     sigaction(SIGINT, &sa_int, NULL);
//     sigaction(SIGQUIT, &sa_quit, NULL);
// }
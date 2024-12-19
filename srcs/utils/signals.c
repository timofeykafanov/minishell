/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:51 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/19 11:27:04 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_exit_status = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	ft_printf("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	handle_child_sigint(int sig)
{
	(void)sig;
	ft_printf("\n", STDOUT_FILENO);
	exit(130);
}

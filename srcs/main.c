/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/04 17:39:45 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

sig_atomic_t	g_signal = 0;

static int	preparsing(t_memory *memory)
{
	lexer(memory);
	if (memory->lexer_error_code == ERROR_CODE_MALLOC)
		return (free_memory(memory), ERROR);
	else if (memory->lexer_error_code == ERROR_CODE_QUOTES)
		return (handle_quote_error(memory));
	if (!memory->tokens)
		return (free_memory(memory), ERROR);
	expand_tokens(memory);
	if (memory->expander_error_code)
		return (handle_expand_error(memory));
	if (memory->error_code == ERROR_CODE_AMBIGUOUS_REDIRECT)
		return (reset_minishell(memory), CONTINUE);
	if (syntax_check(memory))
		return (handle_syntax_error(memory));
	return (SUCCESS);
}

static int	handle_input(t_memory *memory)
{
	int	status;

	if (!(*memory->input))
		return (reset_minishell(memory), CONTINUE);
	add_history(memory->input);
	status = preparsing(memory);
	if (status == ERROR)
		return (ERROR);
	else if (status == CONTINUE)
		return (CONTINUE);
	parse_command(memory);
	if (!var_name_check(memory))
		return (reset_minishell(memory), CONTINUE);
	execute_heredoc(memory);
	if (g_signal == SIGINT)
		return (handle_heredoc_exit(memory));
	execute_commands(memory);
	delete_heredocs(memory);
	reset_minishell(memory);
	return (SUCCESS);
}

static void	handle_exit(t_memory *memory)
{
	int	exit_status;

	ft_printf("exit\n", STDOUT_FILENO);
	exit_status = memory->exit_status;
	free_memory(memory);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	exit(exit_status);
}

static int	run_shell(t_memory *memory)
{
	int	status;

	while (1)
	{
		memory->suffix = ft_strjoin(memory->pwd, "$ ");
		if (!memory->suffix)
			end_shell(memory);
		memory->input = readline(memory->suffix);
		if (g_signal == SIGINT)
		{
			memory->exit_status = 130;
			g_signal = 0;
		}
		if (memory->input)
		{
			status = handle_input(memory);
			if (status == ERROR)
				return (ERROR);
			else if (status == CONTINUE)
				continue ;
		}
		else
			handle_exit(memory);
	}
}

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;

	set_signals(MAIN);
	(void)ac;
	(void)av;
	memory = init_memory(env);
	if (!memory)
		return (ERROR);
	if (run_shell(memory) == ERROR)
		return (ERROR);
}

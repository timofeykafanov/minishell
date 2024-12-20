/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 13:52:19 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

sig_atomic_t g_signal = 0;

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;
	int			exit_status;
	
	set_signals(MAIN);
	(void)ac;
	(void)av;
	memory = init_memory(env);
	if (!memory)
		return (free_memory(memory), ERROR);
	while (1)
	{
		memory->suffix = ft_strjoin(memory->pwd, "$ ");  
		memory->input = readline(memory->suffix);
		if (g_signal == SIGINT) {
			memory->exit_status = 130;
			g_signal = 0;
		}
		if (memory->input)
		{
			add_history(memory->input);
			if (!(*memory->input))
				continue ;
			lexer(memory);
			if (memory->lexer_error_code == ERROR_CODE_MALLOC)
				return (free_memory(memory), ERROR);
			else if (memory->lexer_error_code == ERROR_CODE_QUOTES)
			{
				reset_minishell(memory);
				memory->lexer_error_code = 0;
				continue;
			}
			if (!memory->tokens)
				return (free_memory(memory), ERROR);
			// print_tokens(memory);
			expand_tokens(memory);
			if (memory->expander_error_code)
			{
				print_error_message(EXPANDER, memory);
				free_tokens(memory->tokens);
				memory->tokens = NULL;
				free(memory->suffix);
				if (memory->faulty_variable_name)
				{
					free(memory->faulty_variable_name);
					memory->faulty_variable_name = NULL;	
				}
				if (memory->input)
				{
					free(memory->input);
					memory->input = NULL;
				}
				memory->expander_error_code = 0;
				continue ;
			}
			if (memory->error_code == ERROR_CODE_AMBIGUOUS_REDIRECT)
			{
				reset_minishell(memory);
				continue ;
			}
			// print_tokens(memory);
			if (syntax_check(memory))
			{
				free_tokens(memory->tokens);
				memory->tokens = NULL;
				free(memory->suffix);
				if (memory->input)
				{
					free(memory->input);
					memory->input = NULL;
				}
				continue ;
			}
			parse_command(memory);
			if (!var_name_check(memory))
			{
				reset_minishell(memory);
				if (memory->faulty_variable_name)
				{
					free(memory->faulty_variable_name);
					memory->faulty_variable_name = NULL;	
				}
				memory->expander_error_code = 0;
				continue ;
			}
			execute_heredoc(memory);
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				memory->exit_status = 130;
				delete_heredocs(memory);
				reset_minishell(memory);
				continue ;
			}
			// print_commands(memory);
			execute_commands(memory);
			delete_heredocs(memory);
			reset_minishell(memory);
		}
		else
		{
			ft_printf("exit\n", STDOUT_FILENO);
			exit_status = memory->exit_status;
			free_memory(memory);
			close(1);
			close(0);
			exit(exit_status);
		}
	}
	// return (free_memory(memory), SUCCESS);
}

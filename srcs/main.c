/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/27 14:52:31 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern volatile sig_atomic_t g_exit_status;

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	(void)ac;
	(void)av;
	memory = init_memory(env);
	if (!memory)
		return (free_memory(memory), ERROR);
	while (1)
	{
		memory->suffix = ft_strjoin(memory->pwd, "$ ");  
		memory->input = readline(memory->suffix);
		if (g_exit_status == 130) {
			memory->exit_status = 130;
			g_exit_status = 0;
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
				continue;
			}
			if (!memory->tokens)
				return (free_memory(memory), ERROR);
			// print_tokens(memory);
			// printf("\n");
			// heredoc(memory->tokens->data);
			// memory->input = read_heredoc_content();
			// print_tokens(memory);
			expand_tokens(memory);
			// printf("\n");
			// print_tokens(memory);
			// printf("\n");
			if (syntax_check(memory))
			{
				free_tokens(memory->tokens);
				memory->tokens = NULL;
				free(memory->suffix);
				// free_commands(memory->commands);
				continue ;
			}
			parse_command(memory);
			execute_heredoc(memory);
			// print_commands(memory);
			
			// TODO: redirect heredoc content

			// printf("heredoc count: %d\n", memory->heredocs_count);
			execute_commands(memory);
			delete_heredocs(memory);
			reset_minishell(memory);
			// memory->exit_status = g_exit_status;
		}
		else
		{
			// ft_printf("exit\n", STDOUT_FILENO);
			free_memory(memory);
			exit(memory->exit_status);
			// break ;
		}
	}
	return (free_memory(memory), SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/19 14:26:42 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
			expand_tokens(memory);
			if (syntax_check(memory))
			{
				free_tokens(memory->tokens);
				memory->tokens = NULL;
				free(memory->suffix);
				// free_commands(memory->commands);
				continue ;
			}
			parse_command(memory);
			execute_heredoc(memory); // segfalt after heredoc
			// printf("heredoc count: %d\n", memory->heredocs_count);
			// print_commands(memory);
			execute_commands(memory);
			delete_heredocs(memory);
			reset_minishell(memory);
		}
		else
		{
			ft_printf("exit\n", STDOUT_FILENO);
			free_memory(memory);
			exit(memory->exit_status);
			// break ;
		}
	}
	return (free_memory(memory), SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/29 16:15:49 by tkafanov         ###   ########.fr       */
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
		memory->suffix = ft_strjoin(memory->path, "$ ");
		memory->input = readline(memory->suffix);
		if (memory->input)
		{
			add_history(memory->input);
			if (!(*memory->input))
				continue ;
			if (lexer(memory))
				return (free_memory(memory), ERROR);
			if (!memory->tokens)
				return (free_memory(memory), ERROR);
			// heredoc(memory->tokens->data);
			// memory->input = read_heredoc_content();
			expand_tokens(memory);
			// print_tokens(memory);
			// print_commands(memory);
			parse_command(memory);
			execute_commands(memory);
			free_tokens(memory->tokens);
			memory->tokens = NULL;
			free(memory->suffix);
		}
		else
		{
			ft_printf("exit\n", STDOUT_FILENO);
			break ;
		}
	}
	return (free_memory(memory), SUCCESS);
}

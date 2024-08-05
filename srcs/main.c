/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/05 17:55:50 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

void handle_sigint(int sig) {
    (void)sig;
    printf("\n");
    // Re-display the prompt
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;
	// char		c;

	// memory = malloc(sizeof(*memory));
	// if (!memory)
	// 	return(ERROR);
	// ft_bzero(memory, sizeof(*memory));
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	(void)ac,
	(void)av;
	
	memory = malloc(sizeof(*memory));
	if (!memory)
		return(ERROR);
	ft_bzero(memory, sizeof(*memory));
	create_env(memory, env);
	if (!memory->env)
	return(perror("Failed to copy environment"), free_memory(memory), ERROR);
	
	while (1)
	{
		memory->input = readline("minishell-> ");
		if (memory->input)
		{
			add_history(memory->input);
			printf("You entered: %s\n", memory->input);
			if (lexer(memory))
				return(free_memory(memory), ERROR);
			if (!memory->tokens)
				return(free_memory(memory), ERROR);
			memory->tokens = free_tokens(memory->tokens);
			free(memory->input);
		}
		else
			break ;
	}
	return (free_memory(memory), SUCCESS);
}

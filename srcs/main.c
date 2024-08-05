/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/05 17:58:02 by tkafanov         ###   ########.fr       */
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

void print_history(void) {
    HIST_ENTRY **history_entries = history_list();
    if (history_entries) {
        for (int i = 0; history_entries[i]; i++) {
            printf("%d: %s\n", i + 1, history_entries[i]->line);
        }
    }
}

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	(void)ac,
	(void)av;

	while (1)
	{
		memory = malloc(sizeof(*memory));
		if (!memory)
			return(ERROR);
		ft_bzero(memory, sizeof(*memory));
		create_env(memory, env);
		if (!memory->env)
		return(perror("Failed to copy environment"), free_memory(memory), ERROR);
		memory->input = readline("minishell-> ");
		if (memory->input)
		{
			HIST_ENTRY *last_entry = history_get(history_length);
            if (!last_entry || ft_strncmp(last_entry->line, memory->input, \
				ft_strlen(last_entry->line)) != 0)
                add_history(memory->input);
			if (!(*memory->input))
				continue ;
			printf("You entered: %s\n", memory->input);
			if (lexer(memory))
				return(free_memory(memory), ERROR);
			if (!memory->tokens)
				return(free_memory(memory), ERROR);
			print_tokens(memory);
			free_memory(memory);
			// free_tokens(memory->tokens);
		}
		else
			break ;
	}
	print_history();
	return (free_memory(memory), SUCCESS);
}

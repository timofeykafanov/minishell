/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/06 18:00:35 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;

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
	memory->path = malloc(PATH_MAX);
	if (!memory->path)
		return(perror("Failed to allocate path"), free_memory(memory), ERROR);
	getcwd(memory->path, PATH_MAX);
	while (1)
	{
		memory->suffix = ft_strjoin(memory->path,"$ ");	
		memory->input = readline(memory->suffix);
		if (memory->input)
		{
			HIST_ENTRY *last_entry = history_get(history_length); // forbidden function
            if (!last_entry || ft_strncmp(last_entry->line, memory->input, \
				ft_strlen(last_entry->line)) != 0)
                add_history(memory->input);
			if (!(*memory->input))
				continue ;
			if (lexer(memory))
				return(free_memory(memory), ERROR);
			if (!memory->tokens)
				return(free_memory(memory), ERROR);
			parse_and_execute_tokens(memory->tokens, memory);
			// print_tokens(memory);
			free_tokens(memory->tokens);
			memory->tokens = NULL;
			free(memory->suffix);
		}
		else
			break ;
	}
	print_history();
	return (free_memory(memory), SUCCESS);
}

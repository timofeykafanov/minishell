/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/28 15:18:10 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int	main(int ac, char **av, char **env)
{
	t_memory	*memory;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	(void)ac;
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
		memory->suffix = ft_strjoin(memory->path, "$ ");
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
			// parse_and_execute_tokens(memory->tokens, memory);
			// print_tokens(memory);
			// memory->tokens->data = expand_var(memory, memory->tokens->data);
			// printf("expanded = %s\n", (char*)memory->tokens->data);
			// char *t = ft_strdup(memory->tokens->data);
			// t = expand_double(memory, t);
			// printf("expanded = %s\n", t);
			// free(t);
			// expand_tokens(memory);
			// print_tokens_as_string(memory);
			// heredoc(memory->tokens->data);
			// memory->input = read_heredoc_content();
			// free_tokens(memory->tokens);
			// memory->tokens = NULL;
			// if(lexer(memory))
			// 	return(free_memory(memory), ERROR);
			expand_tokens(memory);
			remove_spaces(memory);
			// print_tokens(memory);
			parse_command(memory);
			execute_commands(memory);
			// print_commands(memory);
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
	// print_history();
	return (free_memory(memory), SUCCESS);
}

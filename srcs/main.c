/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/05 14:54:32 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_memory *memory;
	(void)ac;
	(void)av;
	
	memory = malloc(sizeof(*memory));
	if (!memory)
		return(perror("Failed to create memory"), ERROR);
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
			// printf("You entered: %s\n", memory->input);
			lexer(memory);
			if (!memory->tokens)
				return(perror("Lexing Error"), free_memory(memory), ERROR);
			print_tokens(memory);
			print_env(memory);
    	}
	 }
	return (free_memory(memory), SUCCESS);
}

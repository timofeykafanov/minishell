/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/07/30 12:45:12 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "minishell.h"

int	main(int ac, char **av)
{
	t_memory *memory;
	memory = malloc(sizeof(*memory));
	if (!memory)
		return(ERROR);

	(void)ac,
	(void)av;
	memory->input = readline("minishell-> ");
	if (memory->input)
	{
        add_history(memory->input);
		printf("You entered: %s\n", memory->input);
		lexer(memory);
		if (!memory->tokens)
			return(free_memory(memory), ERROR);
		print_tokens(memory);
    }
	return (free_memory(memory), SUCCESS);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 13:24:05 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/28 14:53:13 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    remove_spaces(t_memory *memory)
{
	t_tokens *current = memory->tokens;
	t_tokens *next;

	while (current != NULL)
	{
		next = current->next;
		if (current->type == T_WHITESPACE)
		{
			if (current == memory->tokens)
				memory->tokens = next;
			if (current->prev != NULL)
				current->prev->next = next;
			if (next != NULL)
				next->prev = current->prev;
			free(current->data);
			free(current);
		}
		current = next;
	}
}
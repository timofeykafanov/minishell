/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:28:46 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/05 09:48:16 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int print_tokens(t_memory *memory)
{
	t_tokens *current;
	int i;

	i = 1;
	current = memory->tokens;
	printf("Tokens:\n");
	while(current)
	{
		printf("Nr: %d\nToken: %s Type: %d\n", i++, (char*)current->data, current->type);
		current = current->next;
	}
	return (1);
}

int get_type(char *s)
{
	if (*s == D_QUOTE)
		return (T_D_QUOTE);
	else if (*s == S_QUOTE)
		return (T_S_QUOTE);
	else if (*s == DASH)
		return (T_OPTION);
	else if (*s == DOLLAR)
		return (T_VAR);
	else if ((*s == R_IN && *(s+1) != R_IN))
		return (T_R_IN);
	else if ((*s == R_OUT && *(s+1) != R_OUT))
        return (T_R_OUT);
	else if ((*s == R_IN && *(s+1) == R_IN))
		return (T_FIND_DELIM);
	else if ((*s == R_OUT && *(s+1) == R_OUT))
        return (T_OUT_APPEND);
	else if (*s == PIPE)
         return (T_PIPE);
	else
		return (T_WORD);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:28:46 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/06 12:38:44 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	print_tokens(t_memory *memory)
// {
// 	t_tokens	*current;
// 	int			i;

// 	i = 1;
// 	current = memory->tokens;
// 	printf("Tokens:\n");
// 	while (current)
// 	{
// 		printf("Nr: %d\nToken: %s Type: %d Quoted : %d\n", i++, \
// 			(char *)current->data, current->type, current->was_quoted);
// 		current = current->next;
// 	}
// 	return (1);
// }

int	get_type(char *s)
{
	if (*s == D_QUOTE)
		return (T_D_QUOTE);
	else if (*s == S_QUOTE)
		return (T_S_QUOTE);
	else if (*s == DASH)
		return (T_OPTION);
	else if (*s == DOLLAR)
		return (T_VAR);
	else if ((*s == R_IN && *(s + 1) != R_IN))
		return (T_R_IN);
	else if ((*s == R_OUT && *(s + 1) != R_OUT))
		return (T_R_OUT);
	else if ((*s == R_IN && *(s + 1) == R_IN))
		return (T_HEREDOC);
	else if ((*s == R_OUT && *(s + 1) == R_OUT))
		return (T_OUT_APPEND);
	else if (*s == SEMICOLON)
		return (T_SEMICOLON);
	else if (*s == PIPE)
		return (T_PIPE);
	else if (is_whitespace(s))
		return (T_WHITESPACE);
	else
		return (T_WORD);
}

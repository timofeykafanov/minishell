/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:58:02 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/19 11:24:47 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tokens	*merge(t_tokens	*current_token)
{
	char		*prev_token_old_data;
	t_tokens	*to_be_freed_token;

	prev_token_old_data = current_token->prev->data;
	current_token->prev->data = ft_strjoin(current_token->prev->data, \
	current_token->data);
	free(prev_token_old_data);
	current_token->prev->type = T_WORD;
	current_token->prev->was_quoted = 1;
	current_token->prev->next = current_token->next;
	if (current_token->next)
		current_token->next->prev = current_token->prev;
	to_be_freed_token = current_token;
	free(current_token->data);
	current_token = current_token->next;
	free(to_be_freed_token);
	return (current_token);
}

void	merge_tokens(t_memory *memory)
{
	t_tokens	*current_token;

	current_token = memory->tokens;
	while (current_token)
	{
		if (to_merge(current_token))
			current_token = merge(current_token);
		else
			current_token = current_token->next;
	}
}

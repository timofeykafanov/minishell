/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:52:26 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/19 11:23:32 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	to_merge(t_tokens	*current_token)
{
	return (current_token->prev && (current_token->type == T_WORD
			|| current_token->type == T_OPTION
			|| current_token->type == T_D_QUOTE
			|| current_token->type == T_S_QUOTE
			|| current_token->type == T_VAR
			|| current_token->type == T_VAR_DEF)
		&& (current_token->prev->type == T_WORD
			|| current_token->prev->type == T_OPTION
			|| current_token->prev->type == T_D_QUOTE
			|| current_token->prev->type == T_S_QUOTE
			|| current_token->prev->type == T_VAR
			|| current_token->prev->type == T_VAR_DEF));
}

bool	is_prev_redirect(t_tokens *token)
{
	return ((token->prev && (token->prev->type == T_R_OUT \
			|| token->prev->type == T_R_IN \
			|| token->prev->type == T_OUT_APPEND)) \
			|| (token->prev && token->prev->type == T_WHITESPACE \
			&& token->prev->prev \
			&& (token->prev->prev->type == T_R_OUT \
			|| token->prev->prev->type == T_R_IN \
			|| token->prev->prev->type == T_OUT_APPEND)));
}

bool	is_prev_heredoc(t_tokens *token)
{
	return ((token->prev && (token->prev->type == T_HEREDOC)) \
			|| (token->prev && token->prev->type == T_WHITESPACE \
			&& token->prev->prev \
			&& (token->prev->prev->type == T_HEREDOC)));
}

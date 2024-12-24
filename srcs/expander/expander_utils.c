/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:39:53 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/24 14:45:48 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_single_quote(char c)
{
	return (c == '\'');
}

int	is_double_quote(char c)
{
	return (c == '\"');
}

int	is_dollar(char c)
{
	return (c == '$');
}

static void	set_name(t_memory *memory, t_tokens *token, char *expanded_var)
{
	memory->faulty_variable_name = ft_strdup(token->data);
	if (!memory->faulty_variable_name)
	{
		free(expanded_var);
		end_shell(memory);
	}
}

bool	handle_redirect(t_tokens *token, t_memory *memory)
{
	char	*expanded_var;

	expanded_var = NULL;
	if (is_prev_heredoc(token))
		return (true);
	if (is_prev_redirect(token))
	{
		expanded_var = find_env_value(memory, token->data + 1);
		if (ft_strncmp(expanded_var, "", 1) == 0)
		{
			set_error_code(EXPANDER, ERROR_CODE_AMBIGUOUS_REDIRECT, memory);
			memory->exit_status = 1;
			if (!memory->faulty_variable_name)
				set_name(memory, token, expanded_var);
			free(expanded_var);
			return (true);
		}
		free(expanded_var);
	}
	return (false);
}

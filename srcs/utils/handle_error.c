/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 20:26:29 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 21:01:19 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern sig_atomic_t	g_signal;

int	handle_expand_error(t_memory *memory)
{
	print_error_message(EXPANDER, memory);
	free_tokens(memory->tokens);
	memory->tokens = NULL;
	free(memory->suffix);
	if (memory->faulty_variable_name)
	{
		free(memory->faulty_variable_name);
		memory->faulty_variable_name = NULL;
	}
	if (memory->input)
	{
		free(memory->input);
		memory->input = NULL;
	}
	memory->expander_error_code = 0;
	return (CONTINUE);
}

int	handle_syntax_error(t_memory *memory)
{
	free_tokens(memory->tokens);
	memory->tokens = NULL;
	free(memory->suffix);
	if (memory->input)
	{
		free(memory->input);
		memory->input = NULL;
	}
	return (CONTINUE);
}

int	handle_quote_error(t_memory *memory)
{
	reset_minishell(memory);
	memory->lexer_error_code = 0;
	return (CONTINUE);
}

int	handle_parsing_error(t_memory *memory)
{
	reset_minishell(memory);
	if (memory->faulty_variable_name)
	{
		free(memory->faulty_variable_name);
		memory->faulty_variable_name = NULL;
	}
	memory->expander_error_code = 0;
	return (CONTINUE);
}

int	handle_heredoc_exit(t_memory *memory)
{
	g_signal = 0;
	memory->exit_status = 130;
	delete_heredocs(memory);
	reset_minishell(memory);
	return (CONTINUE);
}

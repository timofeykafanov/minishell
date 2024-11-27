/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error_message.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:36:46 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/27 19:21:23 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void print_error_message(int segment, t_memory *memory)
{
    if (segment == LEXER)
    {
        if (memory->lexer_error_code == ERROR_CODE_QUOTES)
            ft_putstr_fd(ERROR_MSG_QUOTE, STDERR_FILENO);
    }
    else if (segment == EXPANDER)
    {
        if (memory->expander_error_code == ERROR_CODE_AMBIGUOUS_REDIRECT)
            ft_printf(ERROR_MSG_AMBIGUOUS_REDIRECT, STDERR_FILENO, memory->ambiguous_redirect_name);
    }
}

int set_error_code(int segment, int error_code, t_memory *memory)
{
    if (segment == LEXER)
    {
        if (error_code == ERROR_CODE_QUOTES)
            memory->lexer_error_code = ERROR_CODE_QUOTES;
        else if (error_code == ERROR_CODE_MALLOC)
            memory->lexer_error_code = ERROR_CODE_MALLOC;
    }
    else if (segment == EXPANDER)
    {
        if (error_code == ERROR_CODE_AMBIGUOUS_REDIRECT)
            memory->expander_error_code = ERROR_CODE_AMBIGUOUS_REDIRECT;
    }
    return (1);
}

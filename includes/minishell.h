/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:57 by tkafanov          #+#    #+#             */
/*   Updated: 2024/07/27 12:46:23 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>

# define SUCCESS 0
# define ERROR 1

# define ECHO "echo"
# define CD "cd"
# define PWD "pwd"
# define EXPORT "export"
# define UNSET "unset"
# define ENV "env"
# define EXIT "exit"

# define R_IN "<"
# define R_OUT ">"
# define FIND_DELIMITER "<<"
# define OUT_APPEND ">>"
# define PIPE "|"
# define DOLLAR "$"

# define T_WORD 1
# define T_PIPE 2
# define T_RET_IN 3 // <
# define T_RET_OUT 4 // >


typedef struct s_tokens
{
    void            *data;
    int             type;
    struct s_tokens *next;
}   t_tokens;


typedef struct s_memory
{
    struct s_tokens *tokens;
}   t_memory;




#endif // MINISHELL_H

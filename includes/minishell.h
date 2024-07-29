/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 11:29:57 by tkafanov          #+#    #+#             */
/*   Updated: 2024/07/28 17:52:57 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


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
# define D_QUOTE "\""
# define S_QUOTE "\'"


# define T_D_QUOTE 1
# define T_S_QUOTE 2

// # define T_BUILTIN 1
// # define T_OPTION 2 
// # define T_PIPE 3
// # define T_RET_IN 4 // <
// # define T_RET_OUT 5 // >
// # define T_DELIMITER 6 // <<
// # define T_REDIRECT 7 // >>
// # define T_VAR 8
// # define T_PATH 9


typedef struct s_tokens
{
    void            *data;
    int             type;
    struct s_tokens *next;
}   t_tokens;


typedef struct s_memory
{
    struct s_tokens *tokens;
    char            *input;
}   t_memory;




#endif // MINISHELL_H

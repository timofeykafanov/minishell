/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:05:49 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/05 12:06:18 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

# define SUCCESS 0
# define ERROR 1

# define ECHO "echo"
# define CD "cd"
# define PWD "pwd"
# define EXPORT "export"
# define UNSET "unset"
# define ENV "env"
# define EXIT "exit"

# define R_IN		'<'
# define R_OUT		'>'
# define PIPE		'|'
# define DOLLAR		'$'
# define D_QUOTE	'\"'
# define S_QUOTE	'\''
# define SEMICOLON	';'
# define AMPERSAND	'&'
# define BRACKET_O	'('
# define BRACKET_C	')'
# define DASH		'-'

# define T_WORD			1
# define T_D_QUOTE		2
# define T_S_QUOTE		3
# define T_OPTION		4
# define T_PIPE			5
# define T_VAR			6
# define T_R_IN			7
# define T_R_OUT		8
# define T_FIND_DELIM	9
# define T_OUT_APPEND	10

typedef struct s_tokens
{
	void			*data;
	int				type;
	struct s_tokens	*next;
}	t_tokens;

typedef struct s_memory
{
	struct s_tokens	*tokens;
	char			*input;
}	t_memory;

int		lexer(t_memory *memory);
void	free_memory(t_memory *memory);
void	free_tokens(t_tokens *token);
char	*find_seperator(char *s);
int		skip_non_whitespace(char *s);
int		skip_whitespace(char *s);
int		print_tokens(t_memory *memory);
int		get_type(char *s);

// parser.c

void	parse_tokens(t_tokens *tokens);

#endif // MINISHELL_H

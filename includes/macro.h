/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 21:15:34 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/09 16:50:07 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACRO_H
# define MACRO_H

# define SUCCESS 0
# define ERROR 1
# define CONTINUE 2

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
# define EQUALS		'='

# define T_WORD			1
# define T_D_QUOTE		2
# define T_S_QUOTE		3
# define T_PIPE			5
# define T_VAR			6
# define T_R_IN			7
# define T_R_OUT		8
# define T_HEREDOC		9
# define T_OUT_APPEND	10
# define T_WHITESPACE	12
# define T_VAR_DEF		13

# define PERMISSION_DENIED 126
# define COMMAND_NOT_FOUND 127

# define LEXER 1
# define PARSER 2
# define EXECUTOR 3
# define EXPANDER 4
# define PATH 5

# define MAIN 1
# define CHILD 2
# define HEREDOC 3
# define WAIT 4

# define ERROR_CODE_MALLOC 1
# define ERROR_CODE_GENERAL 7
# define ERROR_CODE_QUOTES 8
# define ERROR_CODE_AMBIGUOUS_REDIRECT 9
# define ERROR_CODE_INVALID_VAR_NAME 10
# define ERROR_CODE_NO_PATH 11

# define ERROR_MSG_HEREDOC "kinkshell: warning: here-document delimited by \
end-of-file (wanted `%s')\n"
# define ERROR_MSG_EXIT_NUM "kinkshell: exit: %s: numeric argument required\n"
# define ERROR_MSG_MALLOC "kinkshell: Memory Allocation Error\n"
# define ERROR_MSG_QUOTE "kinkshell: Syntax Error: Missing Quote\n"
# define ERROR_MSG_AMBIGUOUS_REDIRECT "kinkshell: %s: ambiguous redirect\n"
# define ERROR_MSG_INVALID_VAR_NAME "kinkshell: export: `%s': \
not a valid identifier\n"

#endif // MACRO_H
/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   minishell.h										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: sopperma <sopperma@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/07/26 11:29:57 by tkafanov		  #+#	#+#			 */
/*   Updated: 2024/07/29 17:07:41 by sopperma		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <linux/limits.h>

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
# define T_BRACKETS		11
# define T_SEMICOLON	12
# define T_WHITESPACE	13
# define T_VAR_DEF		14

typedef struct s_tokens
{
	void			*data;
	int				type;
	struct s_tokens	*next;
}   t_tokens;


typedef struct s_command
{
	char				*name;
	char				**args;
	int					type;
	int					pipe[2];
	int					fd_in;
	int					fd_out;
	struct s_command	*next;
}   t_command;

typedef struct s_commands
{
	char				*name;
	int					type;
	bool				main;
	struct s_commands	*next;
	struct s_commands	*args;
	struct s_commands	*prev;
}	t_commands;

typedef struct s_memory
{
	struct s_tokens		*tokens;
	struct s_commands	*commands;
	char				*input;
	char				*path;
	char				*suffix;
	char				**env;
	int					env_lines;
	int					env_space;
	int					exit_status;
}   t_memory;


int	 	lexer(t_memory *memory);
void	free_memory(t_memory *memory);
void	*free_tokens(t_tokens *token);
int	 	print_tokens(t_memory *memory);
int	 	get_type(char *s);
void 	create_env(t_memory *memory, char **env);
int 	print_env(t_memory *memory);
void	add_env_var(t_memory *memory, char *env);
int		print_export(t_memory *memory);
void	unset(t_memory *memory, char *var_name);

//parsin_utils.c

int		is_whitespace(char *s);
int		skip_non_whitespace(char *s);
char 	*skip_whitespace(char *s);
char	*find_seperator(char *s);
char	*find_whitespace(char *s);
char	*is_var_end(char *s);

// signals.c

void handle_sigint(int sig);

// builtins.c

void	print_history(void);
char	*execute_pwd(t_memory *memory);
char	*execute_cd(t_memory *memory, t_commands *cmd);
void	execute_ls(void);

// parser.c

void	parse_and_execute_tokens(t_tokens *tokens, t_memory *memory);

// parser_2_0.c

void	parse_command(t_memory *memory);
void	print_commands(t_memory *memory);

// executor.c

void	execute_commands(t_memory *memory);

// ft_execve.c

char	*ft_execve(t_memory *memory, t_commands *cmd, int input_fd, bool flag);

// freeing.c

void free_env(char **env);

// expander.c
char *expand_double(t_memory *memory, char *s);
char *expand_single(char *s);
char *expand_var(t_memory *memory, char *var);
void *expand_tokens(t_memory *memory);
void print_tokens_as_string(t_memory *memory);

//heredoc.c
void	heredoc(char *delimiter);
char	*read_heredoc_content();

#endif // MINISHELL_H

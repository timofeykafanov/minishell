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

# include "../libft/libft.h"

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <linux/limits.h>

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
# define DASH		'-'
# define EQUALS		'='

# define T_WORD			1
# define T_D_QUOTE		2
# define T_S_QUOTE		3
# define T_OPTION		4
# define T_PIPE			5
# define T_VAR			6
# define T_R_IN			7
# define T_R_OUT		8
# define T_HEREDOC		9
# define T_OUT_APPEND	10
# define T_SEMICOLON	11
# define T_WHITESPACE	12
# define T_VAR_DEF		13

# define COMMAND_NOT_FOUND 127

# define LEXER 1
# define PARSER 2
# define EXECUTOR 3

# define ERROR_CODE_MALLOC 1
# define ERROR_CODE_GENERAL 7
# define ERROR_CODE_QUOTES 8

# define ERROR_MSG_MALLOC "Memory Allocation Error\n"
# define ERROR_MSG_QUOTE "Syntax Error: Missing Quote\n"

typedef struct s_tokens
{
	void			*data;
	int				type;
	int				was_quoted;
	struct s_tokens	*next;
	struct s_tokens	*prev;
}	t_tokens;

typedef struct s_redir_out
{
	char				*file_name;
	char				*heredoc_file_name;
	int					was_quoted;
	int					type;
	struct s_redir_out	*next;
}	t_redir_out;

typedef struct s_command
{
	char				*name;
	char				**args;
	char				**redir_out;
	t_redir_out			*redir_struct;
	int					type;
	char				*path;
	struct s_command	*next;
}	t_command;

typedef struct s_memory
{
	struct s_tokens		*tokens;
	struct s_command	*commands;
	char				**heredocs;
	int					heredocs_count;
	char				*input;
	char				*pwd;
	char				*suffix;
	char				**env;
	int					env_lines;
	int					env_space;
	int					exit_status;
	int					error_code;
	int					lexer_error_code;
}	t_memory;

typedef struct s_parser
{
	t_command			*current_cmd;
	t_command			*prev_cmd;
	t_tokens			*start_token;
	t_tokens			*current_token;
	t_redir_out			*current_redir;
	t_redir_out			*last_redir;
	int					args_count;
	int 				heredoc_count;
}	t_parser;
// lexer.c

void			lexer(t_memory *memory);

// process_token.c

void		*process_token(char *s, t_memory *memory);

// lexer_utils.c

int			get_type(char *s);

// freeing.c

void		free_memory(t_memory *memory);
void		*free_tokens(t_tokens *token);
void		reset_minishell(t_memory *memory);


// print_error_messages.c
void		print_error_message(int segment, t_memory *memory);
int			set_error_code(int segment, int error_code, t_memory *memory);

// parsing_utils.c

int			is_whitespace(char *s);
int			skip_non_whitespace(char *s);
char		*skip_whitespace(char *s);
char		*find_seperator(char *s);
char		*find_whitespace(char *s);
char		*is_var_end(char *s);


// signals.c

void		handle_sigint(int sig);

// builtins_pwd_exit.c

void		execute_pwd(t_memory *memory);
void		execute_exit(t_memory *memory, bool is_redir, int saved_fds[2]);

// builtins_env_unset.c

void		print_env(t_memory *memory);
void		unset(t_memory *memory, char **args);

// builtins_cd.c

void		execute_cd(t_memory *memory, t_command *cmd);

// export.c

void		execute_export(t_memory *memory, char **args);

// export_utils.c

void		print_export(t_memory *memory);

// parser.c

void		parse_and_execute_tokens(t_tokens *tokens, t_memory *memory);

// parser.c

void		parse_command(t_memory *memory);
void		print_commands(t_memory *memory);

// executor.c

void		execute_commands(t_memory *memory);

// handle_redir.c

void		handle_redir(t_command *cmd);

// single.c

void		execute_single_command(t_command *cmd, t_memory *mem);

// first.c

void		execute_first_command(t_command *cmd, t_memory *mem, int fd1[2]);

// next.c

void		execute_next_command(t_command *cmd, t_memory *mem, int fd1[2]);

// last.c

void		execute_last_command(t_command *cmd, t_memory *mem, int fd1[2]);

// freeing.c

void		free_commands(t_command *commands);
void		free_env(char **env);

// expander.c

void		*expand_tokens(t_memory *memory);

// expander_utils.c

int			is_single_quote(char c);
int			is_double_quote(char c);
int			is_dollar(char c);

// expand_var.c

char		*expand_var(t_memory *memory, char *var);

// expand_double.c

char		*expand_double(t_memory *memory, char *s);

// expand_single.c

char		*expand_single(char *s);

// heredoc.c

void		delete_heredocs(t_memory *memory);
void		heredoc(t_memory *memory, t_redir_out *redir, int i);
char		*read_heredoc_content(void);
void		execute_heredoc(t_memory *memory);

// find_path.c

char		*find_path(char *command, t_memory *memory);

// init_memory.c

char		*get_env_var(t_memory *memory, char *var);
t_memory	*init_memory(char **env);

// echo.c

void		echo(char **args);

// executor_utils.c

bool		is_builtin(char *command);
void		execute_builtin(t_command *cmd, t_memory *mem, bool is_redir, \
	int saved_fds[2]);
bool		is_cd_or_exit(char *command);
void		execute_cd_or_exit(t_command *cmd, t_memory *mem);

#endif // MINISHELL_H

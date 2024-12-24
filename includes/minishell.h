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

# include <limits.h>
# include <stdbool.h>
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
# include <sys/stat.h>

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

# define ERROR_MSG_HEREDOC "warning: here-document delimited by \
end-of-file (wanted `%s')\n"
# define ERROR_MSG_EXIT_NUM "kinkshell: exit: %s: numeric argument required\n"
# define ERROR_MSG_MALLOC "kinkshell: Memory Allocation Error\n"
# define ERROR_MSG_QUOTE "kinkshell: Syntax Error: Missing Quote\n"
# define ERROR_MSG_AMBIGUOUS_REDIRECT "kinkshell: %s: ambiguous redirect\n"
# define ERROR_MSG_INVALID_VAR_NAME "kinkshell: export: `%s': \
not a valid identifier\n"

typedef struct s_tokens
{
	void			*data;
	void			*quotes_removed;
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
	char				*env_path;
	bool				is_filename;
	bool				has_child;
	int					exit_status;
	bool				is_redir_out;
	bool				is_redir_in;
	bool				has_redir;
	char				**paths;
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
	char				*oldpwd;
	int					env_lines;
	int					env_space;
	int					exit_status;
	int					error_code;
	int					lexer_error_code;
	int					expander_error_code;
	bool				exit_failed;
	bool				cd_failed;
	bool				is_child;
	char				*faulty_variable_name;
	pid_t				*pid;
	bool				found;
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
	int					heredoc_count;
}	t_parser;

// lexer.c

void		lexer(t_memory *memory);
t_tokens	*variable_split_lexer(t_memory *memory, char *s);

// process_token.c

void		*process_token(char *s, t_memory *memory, bool split);

// lexer_utils.c

int			get_type(char *s);
bool		is_group_identifier(char *s);
int			print_tokens(t_memory *memory);
int			print_expand_var_tokens(t_tokens *token);

// freeing.c

void		free_memory(t_memory *memory);
void		*free_tokens(t_tokens *token);
void		end_shell(t_memory *memory);

// freeing_two.c

void		free_heredocs(t_memory *memory);
void		reset_minishell(t_memory *memory);
void		*free_tokens(t_tokens *token);
void		free_env(char **env);
void		free_redir_struct(t_redir_out *current);

// print_error_messages.c

void		print_error_message(int segment, t_memory *memory);
int			set_error_code(int segment, int error_code, t_memory *memory);

// parsing_utils.c

int			is_whitespace(char *s);
int			skip_non_whitespace(char *s);
char		*skip_whitespace(char *s);
char		*find_seperator(char *s);
bool		is_redirect(int type);
char		*find_whitespace(char *s);
char		*is_var_end(char *s);
bool		is_separator(char c);

// parsing_utils_3.c

int			check_current_token_type(t_parser **p);
t_command	*create_command(char *name, char **args, int type, \
	t_memory *memory);
t_parser	*init_parser(t_memory *memory);
void		parser_init_phase_two(t_parser **p, t_memory *memory);
void		setup_redirect(t_parser *p);

// syntax_check.c

int			syntax_check(t_memory *memory);
bool		var_name_check(t_memory *memory);

// signals.c

void		set_signals(int type);

// exit.c

void		execute_exit(t_memory *memory, t_command *cmd, int saved_fds[2]);

// exit_utils.c

bool		contains_only_spaces(char *str);
bool		contains_only_digits(char *str, int *sign);
bool		check_overflow(int sign, char *str);

// pwd.c

void		execute_pwd(t_memory *memory);

// unset.c

void		unset(t_memory *memory, char **args);

// env.c

void		print_env(t_memory *memory);

// cd.c

void		execute_cd(t_memory *memory, t_command *cmd);

// cd_utils.c

int			find_env_index(char **env, char *var);
void		update_oldpwd(t_memory *memory, char *oldpwd);
void		handle_missing_oldpwd(t_memory *memory);
void		define_pwds(t_memory *memory, char *oldpwd);
void		check_pwds(t_memory *memory);

// export.c

void		execute_export(t_memory *memory, char **args);

// export_utils.c

void		print_export(t_memory *memory);

// export_utils_2.c

void		print_vars(t_memory *mem, char **copy);

// parser.c

void		parse_and_execute_tokens(t_tokens *tokens, t_memory *memory);

// parser.c

void		parse_command(t_memory *memory);
void		print_commands(t_memory *memory);

// executor.c

void		execute_commands(t_memory *memory);

// executor_utils.c

bool		is_builtin(char *command);
void		execute_builtin(t_command *cmd, t_memory *mem, int saved_fds[2]);
void		execute_cd_or_exit(t_command *cmd, t_memory *mem);
bool		is_directory(t_memory *memory, t_command *command);
void		catch_status(t_memory *memory, int *status);

// handle_execution.c

void		handle_execution(t_command *cmd, t_memory *mem);

// expander_checks.c

bool		to_merge(t_tokens	*current_token);
bool		is_prev_redirect(t_tokens *token);
bool		is_prev_heredoc(t_tokens *token);

// merger.c

t_tokens	*merge(t_tokens	*current_token, t_memory *memory);
void		merge_tokens(t_memory *memory);

// handle_redir.c

void		handle_redir_out(t_command *cmd, t_memory *memory, bool has_child);
void		handle_redir_in(t_command *cmd, t_memory *memory, bool has_child);
void		handle_redir(t_command *cmd, t_memory *memory, bool has_child);

// handle_redir_utils.c

void		redir_out(t_redir_out *redir, t_memory *mem, bool has_child);
void		redir_append(t_redir_out *redir, t_memory *mem, bool has_child);
void		redir_in(t_redir_out *redir, t_memory *mem, bool has_child);
void		redir_heredoc(t_redir_out *redir, t_memory *mem, bool has_child);

// has_redir.c

bool		has_redir_in(t_command *cmd);
bool		has_redir_out(t_command *cmd);

// single.c

void		execute_single_command(t_command *cmd, t_memory *mem, int *status);

// single_utils.c

void		execute_builtin_and_handle_redir(t_command *cmd, t_memory *mem, \
	int saved_fds[2]);

// first.c

int			execute_first_command(t_command *cmd, t_memory *mem, int fd1[2]);

// next.c

int			execute_next_command(t_command *cmd, t_memory *mem, int fd1[2]);

// last.c

int			execute_last_command(t_command *cmd, t_memory *mem, int fd1[2]);

// freeing.c

void		free_commands(t_command *commands);
void		free_env(char **env);
void		end_parser_malloc_error(t_memory *memory, t_parser *p);

// expander.c

void		*expand_tokens(t_memory *memory);

// expander_utils.c

int			is_single_quote(char c);
int			is_double_quote(char c);
int			is_dollar(char c);
bool		handle_redirect(t_tokens *token, t_memory *memory);

// expand_var.c

char		*find_env_value(t_memory *memory, const char *key);
char		*expand_var(t_memory *memory, char *var);

// expand_double.c

char		*expand_double(t_memory *memory, char *s);
char		*remove_quotes(char *s);
bool		expand_double_quotes(t_tokens *token, t_memory *memory);

// expand_single.c

char		*expand_single(char *s);

// heredoc.c

void		execute_heredoc(t_memory *memory);

// heredoc_utils.c

void		delete_heredocs(t_memory *memory);
char		*heredoc_expander(t_memory *memory, char *line);

// find_path.c

char		*find_path(char *command, t_memory *memory, t_command *cmd);

// init_memory.c

char		*get_env_var(t_memory *memory, char *var);
t_memory	*init_memory(char **env);

// echo.c

void		echo(char **args);

// handle_error.c

int			handle_expand_error(t_memory *memory);
int			handle_syntax_error(t_memory *memory);
int			handle_quote_error(t_memory *memory);
int			handle_parsing_error(t_memory *memory);
int			handle_heredoc_exit(t_memory *memory);

#endif // MINISHELL_H

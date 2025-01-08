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
# include "macro.h"
# include "structs.h"

# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>

// lexer.c

void		lexer(t_memory *memory);
t_tokens	*variable_split_lexer(t_memory *memory, char *s);

// lexer_utils.c

int			get_type(char *s);
bool		is_group_identifier(char *s);
void		sanitize_tokens(t_memory *memory);

// process_token.c

void		*process_token(char *s, t_memory *memory, bool split);

// process_token_utils.c

void		check_separator(char *s, int *len);

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

int			handle_redir_out(t_command *cmd, t_memory *memory, bool has_child);
int			handle_redir_in(t_command *cmd, t_memory *memory, bool has_child);
int			handle_redir(t_command *cmd, t_memory *memory, bool has_child);

// handle_redir_utils.c

int			redir_out(t_redir_out *redir, t_memory *mem, bool has_child);
int			redir_append(t_redir_out *redir, t_memory *mem, bool has_child);
int			redir_in(t_command *cmd, t_redir_out *redir, t_memory *mem, \
	bool has_child);
int			redir_heredoc(t_command *cmd, t_redir_out *redir, t_memory *mem, \
	bool has_child);

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

char		*find_env_value(t_memory *memory, char *key);
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

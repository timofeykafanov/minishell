/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 21:17:05 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 21:10:26 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

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
	int					len;
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

#endif // STRUCTS_H
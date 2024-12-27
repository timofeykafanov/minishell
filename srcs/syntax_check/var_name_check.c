/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_name_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 11:43:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 19:00:48 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_valid_variable_declaration(char *str)
{
	if (!str || (str && *str == '=') || isdigit(*str))
		return (false);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
		str++;
	}
	return (true);
}

static void	set_var_and_error_code(t_memory *memory, t_command *cmd, int i)
{
	(void)cmd;
	(void)i;
	memory->faulty_variable_name = ft_strdup(cmd->args[i]);
	if (!memory->faulty_variable_name)
		end_shell(memory);
	memory->exit_status = 1;
	set_error_code(EXECUTOR, ERROR_CODE_INVALID_VAR_NAME, memory);
	print_error_message(EXECUTOR, memory);
}

bool	var_name_check(t_memory *memory)
{
	t_command	*cmd;
	int			i;

	cmd = memory->commands;
	while (cmd)
	{
		if (cmd->args[0] && ft_strncmp(cmd->args[0], "export", \
			ft_strlen(cmd->args[0])) == 0 && ft_strlen(cmd->args[0]) == 6)
		{
			i = 1;
			while (cmd->args[i])
			{
				if (!is_valid_variable_declaration(cmd->args[i]))
				{
					set_var_and_error_code(memory, cmd, i);
					return (false);
				}
				i++;
			}
		}
		cmd = cmd->next;
	}
	return (true);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:16:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 11:16:11 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_env_index(char **env, char *var)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static void	define_pwds(t_memory *memory, char *oldpwd)
{
	int	i;

	i = find_env_index(memory->env, "OLDPWD=");
	if (i != -1)
	{
		free(memory->env[i]);
		memory->env[i] = ft_strjoin("OLDPWD=", oldpwd);
	}
	i = find_env_index(memory->env, "PWD=");
	if (i != -1)
	{
		free(memory->env[i]);
		memory->env[i] = ft_strjoin("PWD=", memory->pwd);
	}
}

static void	check_pwds(t_memory *memory)
{
	char	*oldpwd;
	int		i;

	oldpwd = get_env_var(memory, "PWD=");
	getcwd(memory->pwd, PATH_MAX);
	if (!oldpwd)
	{
		i = find_env_index(memory->env, "OLDPWD=");
		if (i != -1)
		{
			while (memory->env[i + 1])
			{
				memory->env[i] = memory->env[i + 1];
				i++;
			}
			memory->env[i] = NULL;
		}
	}
	define_pwds(memory, oldpwd);
}

void	execute_cd(t_memory *memory, t_command *cmd)
{
	if (cmd->args)
	{
		if (chdir(cmd->args[1]) != 0)
		{
			ft_printf("minishell: %s: %s: ", STDERR_FILENO, \
				cmd->args[0], cmd->args[1]);
			perror("");
		}
	}
	check_pwds(memory);
}

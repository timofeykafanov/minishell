/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:16:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/16 19:09:28 by tkafanov         ###   ########.fr       */
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
	if (oldpwd)
	{
		if (memory->oldpwd)
			free(memory->oldpwd);
		memory->oldpwd = ft_strdup(oldpwd);
	}
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
	char	*oldpwd;
	char	*home;

	if (cmd->args)
	{
		if (cmd->args[1] && cmd->args[2])
		{
			ft_printf("%s: too many arguments\n", STDERR_FILENO, \
				cmd->args[0]);
			memory->cd_failed = true;
		}
		else if (cmd->args[1] == NULL)
		{
			home = get_env_var(memory, "HOME=");
			if (!home)
			{
				ft_printf("%s: HOME not set\n", STDERR_FILENO, \
					cmd->name);
				memory->cd_failed = true;
			}
			else if (chdir(home) != 0)
			{
				ft_printf("%s: %s: ", STDERR_FILENO, \
					cmd->args[0], cmd->args[1]);
				perror("");
			}
		}
		else if (ft_strncmp(cmd->args[1], "-", ft_strlen(cmd->args[1])) == 0)
		{
			oldpwd = get_env_var(memory, "OLDPWD=");
			if (!oldpwd)
			{
				if (memory->oldpwd)
				{
					oldpwd = memory->oldpwd;
					if (chdir(oldpwd) != 0)
					{
						ft_printf("%s: %s: ", STDERR_FILENO, \
							cmd->args[0], cmd->args[1]);
						memory->cd_failed = true;
						perror("");
					}
				}
				else
				{
					ft_printf("%s: OLDPWD not set\n", STDERR_FILENO, \
						cmd->name);
					memory->cd_failed = true;
				}
			}
			else
			{
				ft_printf("%s\n", STDOUT_FILENO, oldpwd);
				if (chdir(oldpwd) != 0)
				{
					ft_printf("%s: %s: ", STDERR_FILENO, \
						cmd->args[0], cmd->args[1]);
					memory->cd_failed = true;
					perror("");
				}
			}
		}
		else
		{
			int cd = chdir(cmd->args[1]);
			// printf("cd: %d\n", cd);
			if (cd != 0)
			{
				ft_printf("%s: %s: ", STDERR_FILENO, \
					cmd->args[0], cmd->args[1]);
				memory->cd_failed = true;
				perror("");
			}
		}
	}
	if (!memory->cd_failed)
		check_pwds(memory);
}

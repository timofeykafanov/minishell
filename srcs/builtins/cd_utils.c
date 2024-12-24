/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:35:49 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/24 13:05:59 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	find_env_index(char **env, char *var)
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

void	update_oldpwd(t_memory *memory, char *oldpwd)
{
	if (memory->oldpwd)
		free(memory->oldpwd);
	memory->oldpwd = ft_strdup(oldpwd);
	if (!memory->oldpwd)
		end_shell(memory);
}

void	handle_missing_oldpwd(t_memory *memory)
{
	int	i;

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

void	define_pwds(t_memory *memory, char *oldpwd)
{
	int	i;

	i = find_env_index(memory->env, "OLDPWD=");
	if (i != -1)
	{
		free(memory->env[i]);
		memory->env[i] = ft_strjoin("OLDPWD=", oldpwd);
		if (!memory->env[i])
			end_shell(memory);
	}
	i = find_env_index(memory->env, "PWD=");
	if (i != -1)
	{
		free(memory->env[i]);
		memory->env[i] = ft_strjoin("PWD=", memory->pwd);
		if (!memory->env[i])
			end_shell(memory);
	}
}

void	check_pwds(t_memory *memory)
{
	char	*oldpwd;

	oldpwd = get_env_var(memory, "PWD=");
	if (oldpwd)
		update_oldpwd(memory, oldpwd);
	getcwd(memory->pwd, PATH_MAX);
	if (!oldpwd)
		handle_missing_oldpwd(memory);
	define_pwds(memory, oldpwd);
}

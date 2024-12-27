/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 20:48:47 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 19:20:46 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_var(t_memory *memory, char *var)
{
	int	i;

	i = 0;
	while (memory->env[i])
	{
		if (ft_strncmp(memory->env[i], var, ft_strlen(var)) == 0
			&& memory->env[i][ft_strlen(var)] == '=')
			return (memory->env[i] + ft_strlen(var) + 1);
		i++;
	}
	return (NULL);
}

static void	define_oldpwd(t_memory *memory, char *oldpwd, int i)
{
	free(memory->env[i]);
	memory->env[i] = ft_strjoin("OLDPWD=", oldpwd);
	if (!memory->env[i])
	{
		i++;
		while (memory->env[i])
			free(memory->env[i++]);
		end_shell(memory);
	}
}

static void	define_pwd(t_memory *memory, int i)
{
	free(memory->env[i]);
	memory->env[i] = ft_strjoin("PWD=", memory->pwd);
	if (!memory->env[i])
	{
		i++;
		while (memory->env[i])
			free(memory->env[i++]);
		end_shell(memory);
	}
}

void	define_pwds(t_memory *memory, char *oldpwd)
{
	int	i;

	i = find_env_index(memory->env, "OLDPWD");
	if (i != -1)
		define_oldpwd(memory, oldpwd, i);
	i = find_env_index(memory->env, "PWD");
	if (i != -1)
		define_pwd(memory, i);
}

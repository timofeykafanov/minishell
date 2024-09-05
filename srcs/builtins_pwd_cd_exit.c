/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_cd_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/09/05 13:55:51 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_pwd(t_memory *memory)
{
	getcwd(memory->pwd, PATH_MAX);
	if (memory->pwd != NULL)
		printf("%s\n", memory->pwd);
	else
		perror("minishell: pwd");
}

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

void	execute_cd(t_memory *memory, t_command *cmd)
{
	char	*oldpwd;
	int		i;

	if (cmd->args)
	{
		if (chdir(cmd->args[1]) != 0)
		{
			ft_printf("minishell: %s: %s: ", STDERR_FILENO, \
				cmd->args[0], cmd->args[1]);
			perror("");
		}
	}
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

void	execute_exit(t_memory *memory, bool is_redir, int saved_fds[2])
{
	if (is_redir && saved_fds)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	free_memory(memory);
	printf("exit\n");
	exit(0);
}

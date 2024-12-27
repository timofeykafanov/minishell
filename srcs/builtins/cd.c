/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:16:22 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 14:45:11 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_cd_home(t_memory *memory, t_command *cmd)
{
	char	*home;

	home = get_env_var(memory, "HOME");
	if (!home)
	{
		ft_printf("kinkshell: %s: HOME not set\n", STDERR_FILENO, \
			cmd->name);
		memory->cd_failed = true;
	}
	else if (chdir(home) != 0)
	{
		ft_printf("kinkshell: %s: %s: ", STDERR_FILENO, \
			cmd->args[0], cmd->args[1]);
		perror("");
	}
}

static void	if_no_oldpwd(t_memory *memory, t_command *cmd, char *oldpwd)
{
	if (memory->oldpwd)
	{
		oldpwd = memory->oldpwd;
		if (chdir(oldpwd) != 0)
		{
			ft_printf("kinkshell: %s: %s: ", STDERR_FILENO, \
				cmd->args[0], cmd->args[1]);
			memory->cd_failed = true;
			perror("");
		}
	}
	else
	{
		ft_printf("kinkshell: %s: OLDPWD not set\n", STDERR_FILENO, \
			cmd->name);
		memory->cd_failed = true;
	}
}

static void	handle_cd_back(t_memory *memory, t_command *cmd)
{
	char	*oldpwd;

	oldpwd = get_env_var(memory, "OLDPWD");
	if (!oldpwd)
		if_no_oldpwd(memory, cmd, oldpwd);
	else if (ft_strlen(oldpwd) == 0)
		ft_printf("%s\n", STDOUT_FILENO, oldpwd);
	else
	{
		ft_printf("%s\n", STDOUT_FILENO, oldpwd);
		if (chdir(oldpwd) != 0)
		{
			ft_printf("kinkshell: %s: %s: ", STDERR_FILENO, \
				cmd->args[0], cmd->args[1]);
			memory->cd_failed = true;
			perror("");
		}
	}
}

static void	handle_casual_cd(t_memory *memory, t_command *cmd)
{
	int	cd;

	cd = chdir(cmd->args[1]);
	if (cd != 0)
	{
		ft_printf("kinkshell: %s: %s: ", STDERR_FILENO, \
			cmd->args[0], cmd->args[1]);
		memory->cd_failed = true;
		perror("");
	}
}

void	execute_cd(t_memory *memory, t_command *cmd)
{
	if (cmd->args)
	{
		if (cmd->args[1] && cmd->args[2])
		{
			ft_printf("kinkshell: %s: too many arguments\n", STDERR_FILENO, \
				cmd->args[0]);
			memory->cd_failed = true;
		}
		else if (cmd->args[1] == NULL)
			handle_cd_home(memory, cmd);
		else if (ft_strncmp(cmd->args[1], "-", ft_strlen(cmd->args[1])) == 0)
			handle_cd_back(memory, cmd);
		else
			handle_casual_cd(memory, cmd);
	}
	if (!memory->cd_failed)
		check_pwds(memory);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:30:54 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/28 18:20:58 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_pwd(t_memory *memory)
{
	getcwd(memory->pwd, PATH_MAX);
	if (memory->pwd != NULL)
		ft_printf("%s\n", STDOUT_FILENO, memory->pwd);
	else
		perror("pwd");
}

static bool	contains_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && \
			str[i] != '\v' && str[i] != '\f' && str[i] != '\r')
			return (false);
		i++;
	}
	return (true);
}

static bool	contains_only_digits(char *str, int *sign)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || \
		str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
	{
		*sign = -1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			if (contains_only_spaces(str + i))
				return (true);
			return (false);
		}
		i++;
	}
	return (true);
}

static void	exit_shell(t_memory *memory, bool is_redir, int saved_fds[2])
{
	if (is_redir && saved_fds)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	free_memory(memory);
	exit(memory->exit_status);
}

static bool is_within_long_range(char *str, int sign)
{
    char *max_str = "9223372036854775807";
    char *min_str = "9223372036854775808";
    int len = 0;

	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v' || *str == '\f' || *str == '\r')
		str++;
    if (*str == '+' || *str == '-')
	{
        str++;
	}
	while (*str == '0')
		str++;
    while (str[len])
	{	
        len++;
	}
	while (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\v' || str[len - 1] == '\f' || str[len - 1] == '\r')
		len--;
    if (len > 19)
        return false;
    if (len == 19)
    {
        if (sign == 1)
        {
            for (int i = 0; i < 19; i++)
            {
                if (str[i] > max_str[i])
                    return false;
                if (str[i] < max_str[i])
                    break;
            }
        }
        else
        {
            for (int i = 0; i < 19; i++)
            {
                if (str[i] > min_str[i])
                    return false;
                if (str[i] < min_str[i])
                    break;
            }
        }
    }
    return true;
}

void execute_exit(t_memory *memory, bool is_redir, int saved_fds[2])
{
    t_command *command;
    int sign = 1;

    command = memory->commands;
    if (command->args[1] && contains_only_digits(command->args[1], &sign) && command->args[2])
    {
        ft_printf("exit: too many arguments\n", STDERR_FILENO);
        memory->exit_status = 1;
        memory->exit_failed = true;
        return;
    }
    else if (command->args[1])
    {
        if (contains_only_digits(command->args[1], &sign) && is_within_long_range(command->args[1], sign) && command->args[1][0])
            memory->exit_status = ft_atoi(command->args[1]) % 256;
        else
        {
            ft_printf("exit: %s: numeric argument required\n", STDERR_FILENO, command->args[1]);
            memory->exit_status = 2;
        }
    }
    if (sign == -1)
        memory->exit_status = (256 + memory->exit_status) % 256;
    exit_shell(memory, is_redir, saved_fds);
}
// void	execute_exit(t_memory *memory, bool is_redir, int saved_fds[2])
// {
// 	t_command	*command;
// 	int			sign;

// 	sign = 1;
// 	command = memory->commands;
// 	if (command->args[1] && contains_only_digits(command->args[1], &sign) && command->args[2])
// 	{
// 		// ft_printf("exit\n", STDERR_FILENO);
// 		ft_printf("exit: too many arguments\n", STDERR_FILENO);
// 		memory->exit_status = 1;
// 		memory->exit_failed = true;
// 		return ;
// 	}
// 	else if (command->args[1])
// 	{
// 		if (contains_only_digits(command->args[1], &sign) && command->args[1][0])
// 			memory->exit_status = ft_atoi(command->args[1]) % 256;
// 		else
// 		{
// 			// ft_printf("exit\n", STDERR_FILENO);
// 			ft_printf("exit: %s: numeric argument required\n", \
// 				STDERR_FILENO, command->args[1]);
// 			memory->exit_status = 2;
// 		}
// 	}
// 	// printf("exit status: %d\n", memory->exit_status);
// 	// printf("sign: %d\n", sign);
// 	if (sign == -1)
// 		memory->exit_status = 256 + memory->exit_status;
// 	// printf("exit status: %d\n", memory->exit_status);
// 	exit_shell(memory, is_redir, saved_fds);
// }

// bool	is_valid_number(const char *str)
// {
// 	while (isspace((unsigned char)*str)) // Skip leading whitespace
// 		str++;

// 	if (*str == '+' || *str == '-') 
// 		str++;  // Skip the sign if present

// 	if (!*str) 
// 		return false;  // Only sign or empty input is invalid

// 	while (*str)
// 	{
// 		if (!isdigit((unsigned char)*str))
// 			return false;
// 		str++;
// 	}
// 	return true;
// }

// long	safe_strtol(const char *str, int *status)
// {
// 	unsigned long long	result = 0;
// 	int					sign = 1;

// 	while (isspace((unsigned char)*str)) // Skip leading whitespace
// 		str++;

// 	if (*str == '-')
// 	{
// 		sign = -1;
// 		str++;
// 	}
// 	else if (*str == '+')
// 		str++;
// 	while (*str == '0') 
// 		str++;  // Skip leading zeroes
// 	while (*str)
// 	{
// 		if (result > (unsigned long long)(LONG_MAX / 10) ||
// 			(result == (unsigned long long)(LONG_MAX / 10) && (*str - '0') > (LONG_MAX % 10)))
// 		{
// 			*status = 2;  // Overflow or underflow detected
// 			return 0;
// 		}
// 		result = result * 10 + (*str - '0');
// 		str++;
// 	}
// 	if (sign == -1 && result > (unsigned long long)(-(LONG_MIN + 1)) + 1)
// 	{
// 		*status = 2;  // Underflow for LONG_MIN case
// 		return 0;
// 	}
// 	return sign * (long)result;
// }

// bool	ft_isspace(int c)
// {
// 	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
// }
// void	execute_exit(t_memory *memory, bool is_redir, int saved_fds[2])
// {
// 	t_command *command = memory->commands;
// 	int status = 0;

// 	if (command->args[1] && command->args[2])
// 	{
// 		ft_printf("exit: too many arguments\n", STDERR_FILENO);
// 		memory->exit_status = 1;
// 		memory->exit_failed = true;
// 		return;
// 	}

// 	if (command->args[1])
// 	{
// 		char *arg = command->args[1];
// 		while (ft_isspace((unsigned char)*arg)) arg++;
// 		if (*arg == '\0')
// 		{
// 			ft_printf("exit: : numeric argument required\n", STDERR_FILENO);
// 			memory->exit_status = 2;
// 			exit_shell(memory, is_redir, saved_fds);
// 		}
// 		if (!is_valid_number(arg))
// 		{
// 			ft_printf("exit: %s: numeric argument required\n", STDERR_FILENO, arg);
// 			memory->exit_status = 2;
// 		}
// 		else
// 		{
// 			long value = safe_strtol(arg, &status);
// 			if (status == 2)
// 			{
// 				ft_printf("exit: %s: numeric argument required\n", STDERR_FILENO, arg);
// 				memory->exit_status = 2;
// 			}
// 			else
// 				memory->exit_status = (int)(value % 256);
// 		}
// 	}
// 	exit_shell(memory, is_redir, saved_fds);
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 13:25:02 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 14:55:27 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_special_char(char c)
{
	if (c == '\\' || c == '$' || c == '\"' || c == '`')
		return (1);
	return (0);
}

static void	print(char *var_name, char *var_value)
{
	int	j;

	j = 0;
	ft_printf("declare -x %s=\"", STDOUT_FILENO, var_name);
	while (var_value[j])
	{
		if (is_special_char(var_value[j]))
			ft_printf("\\", STDOUT_FILENO);
		ft_printf("%c", STDOUT_FILENO, var_value[j]);
		j++;
	}
	ft_printf("\"\n", STDOUT_FILENO);
}

static void	check_equal_sign(t_memory *mem, char **copy, int i)
{
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	equal_sign = ft_strchr(copy[i], '=');
	if (equal_sign)
	{
		var_name = ft_strndup(copy[i], equal_sign - copy[i]);
		if (!var_name)
			end_shell(mem);
		var_value = ft_strdup(equal_sign + 1);
		if (!var_value)
		{
			free_env(copy);
			free(var_name);
			end_shell(mem);
		}
		print(var_name, var_value);
		free(var_name);
		free(var_value);
	}
	else
		ft_printf("declare -x %s\n", STDOUT_FILENO, copy[i]);
}

void	print_vars(t_memory *mem, char **copy)
{
	int	i;

	i = 0;
	while (copy[i])
	{
		check_equal_sign(mem, copy, i);
		i++;
	}
}

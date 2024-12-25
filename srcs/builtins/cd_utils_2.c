/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 20:48:47 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/25 20:55:02 by tkafanov         ###   ########.fr       */
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

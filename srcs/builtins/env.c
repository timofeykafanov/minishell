/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:08:52 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 21:08:57 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env(t_memory *memory)
{
	int	i;

	i = 0;
	while (memory->env[i])
	{
		if (ft_strchr(memory->env[i], '='))
			ft_printf("%s\n", STDOUT_FILENO, memory->env[i]);
		i++;
	}
}

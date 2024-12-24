/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 21:09:43 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/24 13:31:47 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_pwd(t_memory *memory)
{
	getcwd(memory->pwd, PATH_MAX);
	if (memory->pwd != NULL)
		ft_printf("%s\n", STDOUT_FILENO, memory->pwd);
	else
		perror("kinkshell: pwd");
}

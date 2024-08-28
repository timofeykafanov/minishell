/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:41:43 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/28 14:56:52 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    echo(char **args)
{
    int i;

    i = 0;
    if (ft_strncmp(args[0], "-n", 2) == 0)
    {
        i++;
        while (args[i])
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf(" ");
            i++;
        }
    }
    else
    {
        while (args[i])
        {
            printf("%s", args[i]);
            if (args[i + 1])
                printf(" ");
            i++;
        }
        ft_putstr_fd("\n", 1);
    }
}
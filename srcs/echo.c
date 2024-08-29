/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:41:43 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/29 13:01:52 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    echo(char **args)
{
    int i;

    i = 1;
    if (ft_strncmp(args[i], "-n", 2) == 0)
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
        printf("\n");
    }
    exit(0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:09:53 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/05 14:16:47 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int print_env(t_memory *memory)
{
	t_env *current;
    
	current = memory->env;
	while(current)
	{
		printf("%s\n", (char*)current->value);
		current = current->next;
	}
	return (1);
}

t_env   *add_env(char *env)
{
    t_env *new;
    int len;

    new = malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    len = ft_strlen(env);
    new->value = ft_strdup(env);
    new->is_exp_var = 0;
    new->is_user_var = 0;
    new->next = NULL;
    return (new);
}

void   create_env(t_memory *memory, char **env)
{
    int i;
    t_env *start;
    t_env *current;

    if (!env || !env[0])
        return ;
    start = add_env(env[0]);
    if (!start)
        return ;
    memory->env = start;
    i = 1;
    current = start;
    while (env[i])
    {
        current->next = add_env(env[i]);
        if (!current->next)
            return (free_memory(memory));
        current = current->next;
        i++;
    }
}

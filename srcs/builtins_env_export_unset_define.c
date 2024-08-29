/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_export_unset_define.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:09:53 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/29 13:23:45 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void bubble_sort_char_array(char **array)
{
    int i;
    int j;
    char *temp;

    i = 0;
    while (array[i])
    {
        j = 0;
        while (array[j])
        {
            if (ft_strncmp(array[i], array[j], ft_strlen(array[i])) < 0)
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
            j++;
        }
        i++;
    }
}
int print_env(t_memory *memory)
{
    int i;

    i = 0;
	while(memory->env[i])
		printf("%s\n", memory->env[i++]);
	return (1);
}

int print_export(t_memory *memory)
{
    int i;
    char **copy;

    i = 0;
    copy = malloc(sizeof(char *) * (memory->env_lines + 1));
    if (!copy)
        return (ERROR);
    while (memory->env[i])
    {
        copy[i] = ft_strdup(memory->env[i]);
        i++;
    }
    copy[i] = NULL;
    bubble_sort_char_array(copy);
    i = 0;
	while(copy[i])
		printf("declare -x %s\n", copy[i++]);
    free_env(copy);
	return (1);
}

void unset(t_memory *memory, char **args)
{
    int i;
    int j;
    
    i = 1;
    j = 0;
    while (args[i])
    {
        while (memory->env[j] && j < memory->env_lines)
        {
            if (ft_strncmp(memory->env[j], args[i], ft_strlen(args[i])) == 0)
            {
                free(memory->env[j]);
                memory->env[j] = NULL;
                break;
            }
            j++;
        }
        i++;
    }
}

void  add_env_var(t_memory *memory, char **args)
{
    int i;
    int j;
    
    j = 1;
    i = 0;
    while (memory->env[i] && i < memory->env_lines)
        i++;
    while (args[j])
    {
        if (memory->env_lines == memory->env_space)
        {
            memory->env = ft_realloc(memory->env, sizeof(char *) * (memory->env_lines + 512 + 1));
            memory->env_space += 512;
        }
        memory->env[i++] = ft_strdup(args[j++]);
        memory->env_lines++;
    }
    memory->env[i] = NULL;
}

void    execute_export(t_memory *memory, char **args)
{
    if (!args[1])
        print_export(memory);
    else
        add_env_var(memory, args);
}

void   create_env(t_memory *memory, char **env)
{
    int i;

    i = 0;
    memory->env = malloc(sizeof(char *) * 512);
    memory->env_lines = 0;
    memory->env_space = 512;
    while (env[i])
    {
        memory->env[i] = ft_strdup(env[i]);
        memory->env_lines++;
        i++;
    }
    memory->env[i] = NULL;
}

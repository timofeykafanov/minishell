/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_export_unset_define.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:09:53 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/08 12:11:28 by sopperma         ###   ########.fr       */
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

int is_single_quote(char c)
{
    return (c == '\'');
}
int is_double_quote(char c)
{
    return (c == '\"');
}

char    *ft_strndup(char *s, int len)
{
    char *res;
    int i;

    res = malloc(sizeof(char) * (len + 1));
    if (!res)
        return (NULL);
    i = 0;
    while (i < len)
    {
        res[i] = s[i];
        i++;
    }
    res[i] = '\0';
    return (res);
}

char *ft_strljoin(char *s1, char *s2, int len)
{
    char *res;
    int i;
    int j;
    if (!s1)
        return (ft_strndup(s2, len));
    res = malloc(sizeof(char) * (ft_strlen(s1) + len + 1));
    if (!res)
        return (NULL);
    i = 0;
    j = 0;
    while (s1[i])
    {
        res[i] = s1[i];
        i++;
    }
    while (j < len)
    {
        res[i] = s2[j];
        i++;
        j++;
    }
    res[i] = '\0';
    free(s1);
    return (res);
}

char *clean_var(char *s)
{
    char *res;
    
    res = NULL;
    while(*s)
    {
        if (is_single_quote(*s))
        {
            s = ft_strljoin(res, s, ft_strchr(s + 1, '\'') - s);
        }
        else
        {
            s = ft_strljoin(res, s, ft_strchr(s, '\'') - s);
        }
    } 
    return (res);
}

void unset(t_memory *memory, char *var_name)
{
    printf("Unsetting %s\n", var_name);
    int i;

    i = 0;
    while (memory->env[i] && i < memory->env_lines)
    {
        if (ft_strncmp(memory->env[i], var_name, ft_strlen(var_name)) == 0)
        {
            free(memory->env[i]);
            memory->env[i] = NULL;
            break;
        }
        i++;
    }
}

int count_env_lines(t_memory *memory)
{
    int i;

    i = 0;
    while (memory->env[i])
        i++;
    return (i);
}

void *ft_realloc(void *ptr, size_t size)
{
    void *res;

    res = malloc(size);
    if (!res)
        return (NULL);
    ft_memcpy(res, ptr, size);
    free(ptr);
    return (res);
}

void  add_env_var(t_memory *memory, char *env_var)
{
    int i;

    i = 0;
    while (memory->env[i] && i < memory->env_lines)
        i++;
    if (memory->env_lines == memory->env_space)
    {
        memory->env = realloc(memory->env, sizeof(char *) * (memory->env_lines + 512));
        memory->env_space += 512;
    }
    memory->env[i] = ft_strdup(env_var);
    memory->env_lines++;
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
    // needs to be sorted onl for export prining
    // bubble_sort_char_array(memory->env);
}

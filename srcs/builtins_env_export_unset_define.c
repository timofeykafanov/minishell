/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_export_unset_define.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:09:53 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/07 17:03:56 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int print_export(t_memory *memory)
{
	t_env *current;
    char *word;
    char *c;

	current = memory->env;
	while(current)
	{
		printf("declare -x ");
        if (current->is_user_var)
        {
            c = (char*)current->value;
            word = ft_strchr(current->value, '=') + 1;
            while (c < word - 1)
            {
                printf("%c", *c);
                c++;
            }
            printf("=\"%s\"\n", word);
        }
        else
		    printf("%s\n", (char*)current->value);
		current = current->next;
	}
	return (1);
}

t_env   *new_env(char *env)
{
    t_env *new;
    int len;

    new = malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    len = ft_strlen(env);
    new->value = ft_strdup(env);
    new->next = NULL;
    return (new);
}

int is_single_quote(char c)
{
    return (c == '\'');
}
int is_double_quote(char c)
{
    return (c == '\"');
}

char *ft_strljoin(char *s1, char *s2, int len)
{
    char *res;
    int i;
    int j;

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
    
    while(*s)
    {
        if (is_single_quote(s))
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

void add_env_var(t_memory *memory, char *env, char env_exp)
{
    t_env *last;

    last = memory->env;
    while (last->next)
        last = last->next;
    last->next = new_env(env);
    last->next->is_user_var = 1;
    last->next->is_global = env_exp;
}

void unset(t_memory *memory, char *var_name)
{
    printf("Unsetting %s\n", var_name);
    t_env *current;
    t_env *prev;

    current = memory->env;
    prev = NULL;
    while (current)
    {
        if (!ft_strncmp(current->value, var_name, ft_strlen(var_name)) && current->is_user_var)
        {
            printf("Found %s\n", current->value);
            if (prev)
                prev->next = current->next;
            else
                memory->env = current->next;
            free(current->value);
            free(current);
            return ;
        }
        prev = current;
        current = current->next;
    }
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/08 12:53:18 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_single_quote(char c)
{
    return (c == '\'');
}
int is_double_quote(char c)
{
    return (c == '\"');
}
int is_dollar(char c)
{
    return (c == '$');
}
char *expand_var(t_memory *memory, char *var)
{
    char *value;
    int i;

    i = 0;
    value = NULL;
    while(memory->env[i])
    {
        if (ft_strncmp(memory->env[i], var, ft_strlen(var)) == 0)
        {
            value = ft_strdup(ft_strchr(memory->env[i], '=') + 1);
            break;
        }
        i++;
    }
    free(var);
    return (value);
}

//todo add length to pointer and free var
char *clean_var(t_memory *memory, char *s)
{
    char *res;
    char *var;
    
    res = NULL;
    while(*s)
    {
        if (is_single_quote(*s))
            res = ft_strljoin(res, s, ft_strchr(s + 1, '\'') - s);
        else if (is_dollar(*s))
        {
            var = ft_strndup(s, find_whitespace(s) - s);
            var = expand_var(memory, var + 1);
            res = ft_strljoin(res, var, ft_strlen(var));
            free(var);
        }
    } 
    return (res);
}